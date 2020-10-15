using Editor.Components;
using Editor.Components.Types;
using Editor.FileExplorer;
using Editor.GameObjects;
using Editor.Helpers;
using Editor.Input;
using Editor.Shaders;
using Editor.Windows;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using Type = Editor.Input.Type;

namespace Editor
{
    public partial class EngineEditor : Form
    {
        private string rootDirectory = @"C:\Users\Administrator\Perforce\STEFF-DESKTOP\Personal\Engine\Dev\Resources";

        public EngineEditor()
        {
            InitializeComponent();
        }

        // Window Events
        private void OnWindowLoaded(object sender, EventArgs e)
        {
            SetDecimalSeparator("."); // Make sure the decimal separator in the editor is a dot and not a comma

            Initialize(RenderPanel.Handle, (uint)RenderPanel.Width, (uint)RenderPanel.Height);
            LoadComponentNames();
            LoadGameObjects();
            UpdateFileExplorer(rootDirectory);

            RenderPanel.Focus();

            // Handles engine game loop (if application is idle)
            Application.Idle += HandleApplicationIdle;
            Application.AddMessageFilter(new MouseMessageFilter(HandleMouseMessageEvents));
        }

        private void HandleMouseMessageEvents(MouseEventArguments args)
        {
            if (ShaderEditor.Visible)
                ShaderEditor.HandleMouseMessageEvents(args);
        }

        private void HandleApplicationIdle(object sender, EventArgs e)
        {
            while (IsApplicationIdle())
            {
                Bridge.Tick();
                UpdateComponentVariables();
            }
        }

        private bool IsApplicationIdle()
        {
            NativeMessage result;
            return Bridge.PeekMessage(out result, IntPtr.Zero, (uint)0, (uint)0, (uint)0) == 0;
        }

        private void UpdateComponentVariables()
        {
            foreach (CheckBox checkBox in ControlHelper.GetChildrenOfType<CheckBox>(PropertiesPanel))
            {
                String[] variableData = checkBox.Name.Split(';');
                EditorType editorType = (EditorType)Enum.Parse(typeof(EditorType), variableData[0]);
                VariableType variableType = (VariableType)Enum.Parse(typeof(VariableType), variableData[1]);
                IntPtr valuePtr = new IntPtr(Convert.ToInt64(variableData[2]));

                checkBox.Checked = (bool)ExposedVariableData.GetData(valuePtr, variableType);
            }

            foreach (TextBox textBox in ControlHelper.GetChildrenOfType<TextBox>(PropertiesPanel))
            {
                if (textBox.Focused) // user is typing something in the textbox, don't update it!
                    continue;

                String[] variableData = textBox.Name.Split(';');
                EditorType editorType = (EditorType)Enum.Parse(typeof(EditorType), variableData[0]);
                VariableType variableType = (VariableType)Enum.Parse(typeof(VariableType), variableData[1]);
                IntPtr valuePtr = new IntPtr(Convert.ToInt64(variableData[2]));

                if (editorType == EditorType.NORMAL)
                    textBox.Text = ExposedVariableData.GetData(valuePtr, variableType).ToString();
                else if (editorType == EditorType.SHADER_FILE)
                    textBox.Text = ShaderCache.GetShaderNameByID((uint)ExposedVariableData.GetData(valuePtr, variableType)) ?? "NOT FOUND";
            }
        }

        private void SetDecimalSeparator(string separator)
        {
            System.Globalization.CultureInfo customCulture = (System.Globalization.CultureInfo)System.Threading.Thread.CurrentThread.CurrentCulture.Clone();
            customCulture.NumberFormat.NumberDecimalSeparator = separator;
            System.Threading.Thread.CurrentThread.CurrentCulture = customCulture;
        }

        // Handles key down
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            KeyInput keyInput = new KeyInput(keyData, Type.Down);
            Bridge.AddKeyInput(ref keyInput);

            if (ShaderEditor.Visible)
                ShaderEditor.HandleKeyEvent(keyData);

            return RenderPanel.Focused; // Absorbs the event if the DirectX panel is focussed (return panel.isfocused)
        }

        private void RenderPanelLostFocus(object sender, EventArgs e)
        {
            Bridge.SetInputEnabled(false);
        }

        private void RenderPanelGotFocus(object sender, EventArgs e)
        {
            Bridge.SetInputEnabled(true);
        }

        private void OnKeyUp(object sender, KeyEventArgs e)
        {
            KeyInput keyInput = new KeyInput(e.KeyCode, Type.Up);
            Bridge.AddKeyInput(ref keyInput);
        }

        private void OnMouseUp(object sender, MouseEventArgs e)
        {
            MouseInput mouseInput = new MouseInput(e.Button, Type.Up, (float)e.X, (float)e.Y);
            Bridge.AddMouseInput(ref mouseInput);
        }

        private void OnMouseDown(object sender, MouseEventArgs e)
        {
            RenderPanel.Focus();
        }

        private void OnDebugStateButtonClick(object sender, EventArgs e)
        {
            Bridge.ToggleDebuggingTools();
        }

        private void Initialize(IntPtr pHandle, uint width, uint height)
        {
            Bridge.Initialize(pHandle, width, height);
        }

        private void LoadComponentNames()
        {
            ListBox listBox = ComponentNamesList;

            foreach (String componentName in Bridge.GetComponentNames())
            {
                listBox.Items.Add(componentName);
            }
        }

        private void OnAddComponentClick(object sender, EventArgs e)
        {
            String selectedItem = (String)ComponentNamesList.SelectedItem;
            if (selectedItem != null && GameObjectNamesList.SelectedItem != null)
            {
                int componentID = Bridge.CreateComponent(((GameObjectData)GameObjectNamesList.SelectedItem).id, selectedItem);
                if (componentID != -1)
                {
                    AddComponentProperties(componentID);
                }
            }
        }

        private void ClearComponentProperties()
        {
            PropertiesPanel.Controls.Clear();
        }

        private void LoadComponentsOfGameObject(uint gameObjectID)
        {
            ClearComponentProperties();

            foreach (int componentID in Bridge.GetGameObjectComponents(gameObjectID))
            {
                AddComponentProperties(componentID);
            }
        }

        private void AddComponentProperties(int componentID)
        {
            KeyValuePair<String, List<ExposedVariableData>> componentData = Bridge.GetComponentData((uint)componentID);

            GroupBox componentGroupBox = new GroupBox();
            componentGroupBox.Text = componentData.Key;
            componentGroupBox.AutoSize = true;
            componentGroupBox.MaximumSize = new Size(PropertiesPanel.Width - SystemInformation.VerticalScrollBarWidth - PropertiesPanel.Margin.Left - PropertiesPanel.Margin.Right - 3, 0);
            componentGroupBox.MinimumSize = new Size(PropertiesPanel.Width - SystemInformation.VerticalScrollBarWidth - PropertiesPanel.Margin.Left - PropertiesPanel.Margin.Right - 3, 0);

            FlowLayoutPanel componentPanel = new FlowLayoutPanel();
            componentPanel.FlowDirection = FlowDirection.TopDown;
            componentPanel.AutoSize = true;
            componentPanel.MaximumSize = new Size(PropertiesPanel.Width - SystemInformation.VerticalScrollBarWidth - PropertiesPanel.Margin.Left - PropertiesPanel.Margin.Right - 3, 0);
            componentPanel.MinimumSize = new Size(PropertiesPanel.Width - SystemInformation.VerticalScrollBarWidth - PropertiesPanel.Margin.Left - PropertiesPanel.Margin.Right - 3, 0);
            componentPanel.Location = new Point(0, TextRenderer.MeasureText(componentGroupBox.Text, componentGroupBox.Font).Height + componentGroupBox.Padding.Top);
            componentPanel.Name = componentID.ToString();

            // Variable panel
            foreach (ExposedVariableData variableData in componentData.Value)
            {
                if (!Enum.IsDefined(typeof(VariableType), variableData.type)) // VariableType is unknown so don't put it in the editor
                    continue;

                FlowLayoutPanel variablePanel = new FlowLayoutPanel();
                variablePanel.BorderStyle = BorderStyle.FixedSingle;
                variablePanel.FlowDirection = FlowDirection.LeftToRight;
                variablePanel.AutoSize = true;
                variablePanel.MaximumSize = new Size(componentPanel.Width - componentPanel.Margin.Left - componentPanel.Margin.Right, 0);
                variablePanel.MinimumSize = new Size(componentPanel.Width - componentPanel.Margin.Left - componentPanel.Margin.Right, 0);
                variablePanel.Name = variableData.name;
                componentPanel.Controls.Add(variablePanel);

                Label variableNameLabel = new Label();
                variableNameLabel.Text = variableData.name;
                variableNameLabel.TextAlign = ContentAlignment.MiddleLeft;
                variableNameLabel.AutoSize = false;
                variableNameLabel.Width = variablePanel.Width / 2;
                variablePanel.Controls.Add(variableNameLabel);

                if (variableData.editorType == EditorType.NORMAL)
                {
                    switch (variableData.type)
                    {
                        case VariableType.BOOL:
                        {
                            CheckBox variableValueField = new CheckBox();
                            variableValueField.Checked = ExposedVariableData.GetBool(variableData.pData);
                            variableValueField.Enabled = variableData.accessibility == VariableAccessibility.READWRITE;
                            variableValueField.CheckedChanged += OnValueChange;
                            variableValueField.Name = Convert.ToString(variableData.editorType) + ";" + Convert.ToString(variableData.type) + ";" + variableData.pData.ToInt64().ToString();
                            variablePanel.Controls.Add(variableValueField);

                            variableNameLabel.Height = variablePanel.Height;

                            break;
                        }
                        case VariableType.DIRECTX_XMFLOAT3:
                        case VariableType.XMFLOAT3:
                        {
                            Vec3 vec3 = ExposedVariableData.GetVec3(variableData.pData);

                            for (int i = 0; i < 3; ++i)
                            {
                                TextBox variableValueField = new TextBox();
                                variableValueField.Text = vec3.data[i].ToString();
                                variableValueField.Enabled = variableData.accessibility == VariableAccessibility.READWRITE;
                                variableValueField.KeyUp += OnTextBoxEnterPress;
                                variableValueField.LostFocus += OnValueChange;
                                variableValueField.Name = Convert.ToString(variableData.editorType) + ";" + Convert.ToString(VariableType.FLOAT) + ";" + IntPtr.Add(variableData.pData, 4 * i).ToInt64().ToString();
                                variableValueField.Width = ((variablePanel.Width / 2) / 3) - (variableValueField.Margin.Left * 3);
                                variablePanel.Controls.Add(variableValueField);
                            }

                            break;
                        }
                        default:
                        {
                            TextBox variableValueField = new TextBox();
                            variableValueField.Text = variableData.GetData().ToString();
                            switch (variableData.type)
                            {
                                case VariableType.CHAR:
                                case VariableType.SIGNED_CHAR:
                                case VariableType.UNSIGNED_CHAR:
                                case VariableType.CHAR16_T:
                                    variableValueField.MaxLength = 1;
                                    break;
                            }
                            variableValueField.Enabled = variableData.accessibility == VariableAccessibility.READWRITE;
                            variableValueField.KeyUp += OnTextBoxEnterPress;
                            variableValueField.LostFocus += OnValueChange;
                            variableValueField.Name = Convert.ToString(variableData.editorType) + ";" + Convert.ToString(variableData.type) + ";" + variableData.pData.ToInt64().ToString();
                            variablePanel.Controls.Add(variableValueField);

                            variableNameLabel.Height = variablePanel.Height;

                            break;
                        }
                    }
                }
                else if (variableData.editorType == EditorType.SHADER_FILE)
                {
                    // Textbox will display the name of the shader (but the ID will be send to the engine)
                    TextBox variableValueField = new TextBox();
                    variableValueField.Text = ShaderCache.GetShaderNameByID((uint)variableData.GetData()) ?? "NOT FOUND";
                    variableValueField.Enabled = false;
                    variableValueField.Name = Convert.ToString(variableData.editorType) + ";" + Convert.ToString(variableData.type) + ";" + variableData.pData.ToInt64().ToString();
                    variablePanel.Controls.Add(variableValueField);

                    variableNameLabel.Height = variablePanel.Height;

                    // Shader selector button
                    Button shaderSelectorButton = new Button();
                    shaderSelectorButton.AutoSize = false;
                    shaderSelectorButton.Size = new Size(variableValueField.Height, variableValueField.Height);
                    shaderSelectorButton.BackgroundImageLayout = ImageLayout.Stretch;
                    shaderSelectorButton.BackgroundImage = Image.FromFile(@"C:\Users\Administrator\Perforce\STEFF-DESKTOP\Personal\Engine\Dev\Resources\Textures\ExternalIcon.png");
                    shaderSelectorButton.MouseClick += ShowShaderSelectionButtonPressed;
                    shaderSelectorButton.Tag = variableValueField;
                    variablePanel.Controls.Add(shaderSelectorButton);
                }

                componentGroupBox.Controls.Add(componentPanel);
                PropertiesPanel.Controls.Add(componentGroupBox);
            }
        }

        private void ShowShaderSelectionButtonPressed(object sender, MouseEventArgs e)
        {
            Control control = (Control)sender;
            ShowSelectionBox(control.PointToScreen(e.Location), new Size(150, 300), ShaderHelper.GetShaderNames().ToList<object>(), ShaderSelectionBoxDoubleClick, true, (TextBox)control.Tag);
        }

        private void ShowSelectionBox(Point position, Size size, List<object> items, EventHandler handler, bool doubleClick, object tag)
        {
            int border = 25;

            Point location = PointToClient(position);
            if (location.X + size.Width > Width)
                location.X = Width - size.Width - border;
            if (location.Y + size.Height > Height)
                location.Y = Height - size.Height - border;

            ListBox listBox = new ListBox();
            listBox.Size = size;
            listBox.Location = location;
            listBox.BorderStyle = BorderStyle.FixedSingle;
            listBox.LostFocus += SelectionBoxFocusLost;
            if (doubleClick)
                listBox.DoubleClick += handler;
            else
                listBox.Click += handler;
            listBox.Tag = tag;
            foreach (string item in items)
            {
                listBox.Items.Add(item);
            }
            Controls.Add(listBox);
            listBox.BringToFront();
            listBox.Focus();
        }

        private void ShaderSelectionBoxDoubleClick(object sender, EventArgs args)
        {
            ListBox listBox = (ListBox)sender;

            string selectedShaderName = (string)listBox.SelectedItem;
            if (selectedShaderName != null)
            {
                TextBox shaderTextBox = (TextBox)listBox.Tag;

                // name = editor type;variable type;memory address of the value
                String[] variableData = shaderTextBox.Name.Split(';');
                VariableType variableType = (VariableType)Enum.Parse(typeof(VariableType), variableData[1]);
                IntPtr variableValuePtr = new IntPtr(Convert.ToInt64(variableData[2]));

                // change the value of the ptr to the new value
                shaderTextBox.Text = ExposedVariableData.SetData(variableValuePtr, variableType, ShaderCache.GetShaderIDByName(selectedShaderName).ToString());

                // parent.parent.name = name of componentPanel
                // parent.name = name of variablePanel
                Bridge.VariableChanged(Convert.ToUInt32(shaderTextBox.Parent.Parent.Name), shaderTextBox.Parent.Name);
            }

            listBox.Parent.Controls.Remove(listBox);
        }

        private void SelectionBoxFocusLost(object sender, EventArgs args)
        {
            ListBox listBox = (ListBox)sender;
            listBox.Parent.Controls.Remove(listBox);
        }

        private void OnTextBoxEnterPress(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                ((TextBox)sender).Parent.Focus();
        }

        private void OnValueChange(object sender, EventArgs e)
        {
            Control senderControl = (Control)sender;

            // name = editor type;variable type;memory address of the value
            String[] variableData = senderControl.Name.Split(';');
            VariableType variableType = (VariableType)Enum.Parse(typeof(VariableType), variableData[1]);
            IntPtr variableValuePtr = new IntPtr(Convert.ToInt64(variableData[2]));

            switch (variableType)
            {
                case VariableType.BOOL:
                    ((CheckBox)sender).Checked = Convert.ToBoolean(ExposedVariableData.SetData(variableValuePtr, variableType, ((CheckBox)sender).Checked.ToString()));
                    break;
                default:
                    ((TextBox)sender).Text = ExposedVariableData.SetData(variableValuePtr, variableType, ((TextBox)sender).Text);
                    break;
            }

            // parent.parent.name = name of componentPanel
            // parent.name = name of variablePanel
            Bridge.VariableChanged(Convert.ToUInt32(senderControl.Parent.Parent.Name), senderControl.Parent.Name);
        }

        private void LoadGameObjects()
        {
            GameObjectNamesList.Items.Clear();

            foreach (GameObjectData gameObjectData in Bridge.GetGameObjectsData())
            {
                AddGameObject(gameObjectData);
            }
        }

        private void UpdateFileExplorer(string folderPath)
        {
            // Clear previous entries
            ExplorerFlowLayout.Controls.Clear();
            ExplorerFlowLayout.Tag = new FileData(FileType.FOLDER, folderPath, null);

            // Go back folder
            if (FileHelper.NormalizePath(folderPath) != FileHelper.NormalizePath(rootDirectory))
                AddExplorerItem("...", new FileData(FileType.FOLDER, Path.GetDirectoryName(folderPath), "..."));

            // Folders
            foreach (string folderFilePath in Directory.GetDirectories(folderPath))
            {
                string directoryName = Path.GetFileName(folderFilePath);
                AddExplorerItem(directoryName, new FileData(FileType.FOLDER, folderFilePath, null));
            }

            // Files
            foreach (string shaderFilePath in Directory.GetFiles(folderPath, "*.sSE", SearchOption.TopDirectoryOnly))
            {
                string shaderFileName = Path.GetFileNameWithoutExtension(shaderFilePath);

                uint shaderID = 0;
                foreach (string line in File.ReadLines(shaderFilePath))
                {
                    if (line.StartsWith("ID: "))
                        shaderID = Convert.ToUInt32(line.Substring(line.IndexOf(' ')));
                }

                AddExplorerItem(shaderFileName, new FileData(FileType.SHADER_FILE, shaderFilePath, shaderID));
            }
        }

        private void AddGameObject(GameObjectData gameObjectData)
        {
            GameObjectNamesList.Items.Add(gameObjectData);
        }

        private void GameObjectSelectionChanged(object sender, EventArgs e)
        {
            ListBox gameObjectNamesList = (ListBox)sender;

            if (gameObjectNamesList.SelectedItem == null)
                return;

            GameObjectData gameObjectData = (GameObjectData)gameObjectNamesList.SelectedItem;
            LoadComponentsOfGameObject(gameObjectData.id);
        }

        private void OnCreateGameObjectClick(object sender, EventArgs e)
        {
            AddGameObject(Bridge.CreateGameObject());
        }

        private void OnGameObjectsDoubleClick(object sender, MouseEventArgs e)
        {
            ListBox gameObjectNamesList = (ListBox)sender;

            if (gameObjectNamesList.SelectedItem == null)
                return;

            Rectangle rectangle = gameObjectNamesList.GetItemRectangle(gameObjectNamesList.SelectedIndex);

            TextBox editNameTextBox = new TextBox();
            editNameTextBox.Location = new Point(rectangle.X, rectangle.Y);
            editNameTextBox.Size = new Size(rectangle.Width, rectangle.Height);
            editNameTextBox.Text = ((GameObjectData)gameObjectNamesList.SelectedItem).name;
            editNameTextBox.KeyUp += OnGameObjectNameEditKeyUp;
            editNameTextBox.LostFocus += OnGameObjectNameEditFocusLost;
            gameObjectNamesList.Controls.Add(editNameTextBox);
            editNameTextBox.Focus();
        }

        private void OnGameObjectNameEditKeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                ApplyName(sender);
        }

        private void OnGameObjectNameEditFocusLost(object sender, EventArgs e)
        {
            ApplyName(sender);
        }

        private void ApplyName(object sender)
        {
            GameObjectNamesList.Controls.Remove((Control)sender);

            String name = ((TextBox)sender).Text;
            if (name.Length > 0)
                ChangeGameObjectName(name);
        }

        private void ChangeGameObjectName(String name)
        {
            if (GameObjectNamesList.SelectedItem == null)
                return;

            GameObjectData gameObjectData = (GameObjectData)GameObjectNamesList.SelectedItem;
            gameObjectData.name = name;

            int index = GameObjectNamesList.SelectedIndex;
            GameObjectNamesList.Items.RemoveAt(index);
            GameObjectNamesList.Items.Insert(index, gameObjectData);
            GameObjectNamesList.SelectedIndex = index;
        }

        private void OpenShaderEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (ShaderEditor.Parent != null && ShaderEditor.ParentForm.Name == "StandaloneShaderEditor") // standalone shader editor open - just return
                return;

            ShaderEditor.Visible = !ShaderEditor.Visible;
            ShaderEditor.BringToFront();
            ShaderEditor.Focus();
        }

        private void CompileShaderEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!ShaderEditor.Visible)
                return;

            ShaderEditor.Compile();
            ShaderEditor.Focus();
        }

        private void AddExplorerItem(string fileName, FileData fileData)
        {
            FlowLayoutPanel filePanel = new FlowLayoutPanel();
            filePanel.AutoSize = false;
            filePanel.Size = new Size(85, 86);
            filePanel.Margin = new Padding(9, 3, 9, 3);
            filePanel.BorderStyle = BorderStyle.None;
            filePanel.FlowDirection = FlowDirection.TopDown;
            filePanel.Tag = fileData;
            filePanel.MouseClick += ClickedFile;
            filePanel.MouseDoubleClick += DoubleClickedFile;
            if (fileData.fileType == FileType.FOLDER)
            {
                filePanel.AllowDrop = true;
                filePanel.DragEnter += ExplorerDragEnter;
                filePanel.DragDrop += ExplorerDragDrop;
            }
            filePanel.GotFocus += OnFileFocusChanged;
            filePanel.LostFocus += OnFileFocusChanged;
            filePanel.Paint += PaintExplorerItem;
            filePanel.PreviewKeyDown += DeleteFileFromExplorer;

            PictureBox fileImage = new PictureBox();
            fileImage.Size = new Size(64, 64);
            fileImage.Margin = new Padding(9, 3, 9, 3);
            switch (fileData.fileType)
            {
                case FileType.FOLDER:
                    fileImage.BackgroundImage = Image.FromFile(@"C:\Users\Administrator\Perforce\STEFF-DESKTOP\Personal\Engine\Dev\Resources\Textures\FolderIcon.png");
                    break;
                default:
                    fileImage.BackgroundImage = Image.FromFile(@"C:\Users\Administrator\Perforce\STEFF-DESKTOP\Personal\Engine\Dev\Resources\Textures\FileIcon.png");
                    break;
            }
            fileImage.BackgroundImageLayout = ImageLayout.Stretch;
            fileImage.Tag = fileData;
            fileImage.MouseClick += ClickedFile;
            fileImage.MouseDoubleClick += DoubleClickedFile;

            Label fileNameLabel = new Label();
            fileNameLabel.AutoSize = false;
            fileNameLabel.Size = new Size(83, 14);
            fileNameLabel.Margin = new Padding(1, 0, 1, 0);
            fileNameLabel.Font = new Font("Verdana", 7, FontStyle.Regular);
            fileNameLabel.TextAlign = ContentAlignment.MiddleCenter;
            fileNameLabel.AutoEllipsis = true;
            fileNameLabel.Text = fileName;
            fileNameLabel.Tag = fileData;
            fileNameLabel.MouseClick += ClickedFile;
            fileNameLabel.MouseDoubleClick += RenameExplorerFile;

            filePanel.Controls.Add(fileImage);
            filePanel.Controls.Add(fileNameLabel);

            ExplorerFlowLayout.Controls.Add(filePanel);
        }

        private void ClickedFile(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (sender is FlowLayoutPanel)
                    ((Control)sender).Focus(); // if flowlayoutpanel, it's the file panel itself
                else
                    ((Control)sender).Parent.Focus(); // if something else, it's either the image or the label so get the parent to get the file panel
            }
        }

        private void OnFileFocusChanged(object sender, EventArgs e)
        {
            ((Control)sender).Refresh();
        }

        private void DoubleClickedFile(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                FileData fileData = (FileData)((Control)sender).Tag;
                if (fileData.fileType == FileType.FOLDER)
                    UpdateFileExplorer(fileData.filePath);
            }
        }

        private void ExplorerDragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetFormats().Contains(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Copy;
                ((Control)sender).Focus();
            }
        }

        private void ExplorerDragDrop(object sender, DragEventArgs e)
        {
            foreach (string filePath in e.Data.GetData(DataFormats.FileDrop) as string[])
            {
                string folderPath = ((FileData)((Control)sender).Tag).filePath;
                string movePath = Path.Combine(folderPath, Path.GetFileName(filePath));
                File.Copy(filePath, movePath);

                string currentExplorerPath = ((FileData)ExplorerFlowLayout.Tag).filePath;
                if (FileHelper.NormalizePath(currentExplorerPath) == FileHelper.NormalizePath(folderPath)) // file is dropped in the same directory as the explorer is in
                    UpdateFileExplorer(currentExplorerPath);
                else // file is dropped in a subdirectory of the directory the explorer is in
                    ((Control)sender).Focus();
            }
        }

        private void PaintExplorerItem(object sender, PaintEventArgs e)
        {
            Control control = (Control)sender;

            if (control.Focused)
                e.Graphics.DrawRectangle(new Pen(Color.Red), new Rectangle(0, 0, control.Width - 1, control.Height - 1));
        }

        private void DeleteFileFromExplorer(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
            {
                foreach (Control filePanel in ExplorerFlowLayout.Controls)
                {
                    if (filePanel.Focused)
                    {
                        FileData fileData = (FileData)filePanel.Tag;
                        if (fileData.fileType == FileType.FOLDER && fileData.data != null) // it's the ... folder, do not allow deletion of this
                            return;

                        DialogResult result = MessageBox.Show("You are about to delete '" + Path.GetFileNameWithoutExtension(fileData.filePath) + "'.\nAre you sure you wanna continue?", "File deletion", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                        if (result == DialogResult.Yes)
                        {
                            if (fileData.fileType == FileType.FOLDER)
                                Directory.Delete(fileData.filePath, true);
                            else
                                File.Delete(fileData.filePath);

                            UpdateFileExplorer(((FileData)ExplorerFlowLayout.Tag).filePath);
                        }

                        break;
                    }
                }
            }
        }

        private void ExplorerMouseClick(object sender, MouseEventArgs args)
        {
            if (args.Button == MouseButtons.Right)
            {
                ShowSelectionBox(((Control)sender).PointToScreen(args.Location), new Size(100, 35), new List<object>(new object[] { "Create folder", "Refresh" }), ExplorerSelectionBoxDoubleClick, false, null);
            }
        }

        private void ExplorerSelectionBoxDoubleClick(object sender, EventArgs args)
        {
            ListBox listBox = (ListBox)sender;

            int selectedIndex = listBox.SelectedIndex;
            switch (selectedIndex)
            {
                case 0: // Create folder
                {
                    string explorerFolderPath = ((FileData)ExplorerFlowLayout.Tag).filePath;
                    string newFolderPath = Path.Combine(explorerFolderPath, "New folder");
                    if (Directory.Exists(newFolderPath))
                    {
                        newFolderPath += " 1";

                        int count = 1;
                        while (Directory.Exists(newFolderPath))
                        {
                            newFolderPath = newFolderPath.Remove(newFolderPath.Length - 1) + (count++).ToString();
                        }
                    }

                    Directory.CreateDirectory(newFolderPath);
                    UpdateFileExplorer(explorerFolderPath);
                    break;
                }
                case 1: // Refresh
                {
                    UpdateFileExplorer(((FileData)ExplorerFlowLayout.Tag).filePath);
                    break;
                }
            }

            listBox.Parent.Controls.Remove(listBox);
        }

        private void RenameExplorerFile(object sender, MouseEventArgs args)
        {
            Label control = (Label)sender;
            Control parent = control.Parent;

            TextBox nameTextBox = new TextBox();
            nameTextBox.AutoSize = false;
            nameTextBox.Size = new Size(control.Width, control.Height + 2);
            nameTextBox.Location = control.Location;
            nameTextBox.Margin = new Padding(1, 0, 1, 0);
            nameTextBox.Text = control.Text;
            nameTextBox.Font = new Font("Verdana", 7, FontStyle.Regular);
            nameTextBox.TextAlign = HorizontalAlignment.Center;
            nameTextBox.Tag = control;
            nameTextBox.KeyUp += RenameTextBoxKeyPress;
            //nameTextBox.LostFocus += 
            parent.Controls.Remove(control);
            parent.Controls.Add(nameTextBox);
            nameTextBox.Focus();
        }

        private void RenameTextBoxKeyPress(object sender, KeyEventArgs args)
        {
            TextBox nameTextBox = (TextBox)sender;
            Control parent = nameTextBox.Parent;

            if (args.KeyCode == Keys.Enter) // Submit the new name
            {
                Label nameLabel = (Label)nameTextBox.Tag;
                nameLabel.Text = nameTextBox.Text;

                // Renaming the directory
                FileData fileData = (FileData)nameLabel.Tag;
                if (fileData.fileType == FileType.FOLDER)
                {
                    string newFolderPath = Path.Combine(Path.GetDirectoryName(fileData.filePath), nameLabel.Text);
                    Directory.Move(fileData.filePath, newFolderPath);

                    fileData.filePath = newFolderPath;
                }
                else
                {
                    string newFilePath = Path.Combine(Path.GetDirectoryName(fileData.filePath), nameLabel.Text + Path.GetExtension(fileData.filePath));
                    File.Move(fileData.filePath, newFilePath);

                    fileData.filePath = newFilePath;
                }

                parent.Controls.Remove(nameTextBox);
                parent.Controls.Add((Control)nameTextBox.Tag);

                UpdateFileExplorer(((FileData)ExplorerFlowLayout.Tag).filePath);
            }
            else if (args.KeyCode == Keys.Escape) // Cancel the new name
            {
                parent.Controls.Remove(nameTextBox);
                parent.Controls.Add((Control)nameTextBox.Tag);
            }
        }

        private void StandaloneShaderEditorToolStripMenuItem_Click(object sender, EventArgs args)
        {
            foreach (Form form in OwnedForms)
            {
                if (form.Name == "StandaloneShaderEditor") // Form is already open, close it and just return
                {
                    form.Close();
                    return;
                }
            }

            Form standaloneShaderEditor = new Form();
            standaloneShaderEditor.AutoSize = true;
            standaloneShaderEditor.Name = "StandaloneShaderEditor";
            standaloneShaderEditor.Controls.Add(ShaderEditor);
            standaloneShaderEditor.Show();
            standaloneShaderEditor.FormClosed += StandaloneShaderEditorClosed;
            AddOwnedForm(standaloneShaderEditor);

            ShaderEditor.Visible = true;
        }

        private void StandaloneShaderEditorClosed(object sender, EventArgs args)
        {
            Form standaloneShaderEditor = (Form)sender;
            standaloneShaderEditor.Controls.Remove(ShaderEditor);
            Controls.Add(ShaderEditor);
            ShaderEditor.Visible = false;
        }
    }
}
