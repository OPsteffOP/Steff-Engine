using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using Editor.Input;
using Editor.Shaders;
using System.Drawing.Drawing2D;
using System.Text.Json;
using System.Text.Json.Serialization;
using Editor.Helpers;
using System.Text.RegularExpressions;
using System.Dynamic;
using System.IO;

namespace Editor
{
    public partial class ShaderEditor : UserControl
    {
        FlowLayoutPanel outputNode;

        private List<ShaderNodeData> nodeData = new List<ShaderNodeData>();

        private bool isMouseDraggingPanel = false;
        private bool isMouseDraggingNode = false;
        private Point previousMouseLocation;
        private Control movingNode;

        FlowLayoutPanel creationNode;

        private List<RadioButton> pins = new List<RadioButton>();
        private RadioButton selectedPin;

        private Control selectedNode;

        public ShaderEditor()
        {
            InitializeComponent();

            LoadNodes();
            NodesPanel.Paint += PaintConnectionLines;
            CreateOutputNode();
        }

        private Regex subfolderRegex = new Regex(@"^.*\\(.*)\\.*.json$", RegexOptions.Compiled);
        private void LoadNodes()
        {
            string jsonDirectoryPath = @"C:\Users\Administrator\Perforce\STEFF-DESKTOP\Personal\Engine\Dev\Resources\ShaderEditor\Nodes\";
            foreach (string jsonFilePath in Directory.GetFiles(jsonDirectoryPath, "*.json", SearchOption.AllDirectories))
            {
                string json = File.ReadAllText(jsonFilePath);
                JsonSerializerOptions options = new JsonSerializerOptions();
                options.Converters.Add(new JsonStringEnumConverter(JsonNamingPolicy.CamelCase));
                List<ShaderNodeData> shaderNodeData = JsonSerializer.Deserialize<List<ShaderNodeData>>(json, options);

                nodeData = nodeData.Concat(shaderNodeData).ToList();
                Match match = subfolderRegex.Match(jsonFilePath);
                if (match.Success && match.Groups.Count == 2 && match.Groups[1].Captures.Count == 1)
                {
                    TreeNode parentTreeNode;

                    TreeNode[] foundTreeNodes = NodeSelector.Nodes.Find(match.Groups[1].Captures[0].Value, false);
                    if (foundTreeNodes.Length != 0)
                        parentTreeNode = foundTreeNodes[0]; // Parent tree node already exists - use this one (happens if the subdirectory contains multiple json files)
                    else
                        parentTreeNode = NodeSelector.Nodes.Add(match.Groups[1].Captures[0].Value, match.Groups[1].Captures[0].Value); // Parent tree node doesn't yet exists - create it

                    foreach (ShaderNodeData data in shaderNodeData)
                    {
                        TreeNode treeNode = parentTreeNode.Nodes.Add(data.ToString());
                        treeNode.Tag = data;
                    }
                }
            }
        }

        private void CreateOutputNode()
        {
            List<ShaderPinData> pinData = new List<ShaderPinData>(2);

            ShaderPinData colorPinData = new ShaderPinData("Color", PinType.IN, ShaderVariableType.FLOAT4, ShaderType.PIXEL_SHADER);
            colorPinData.ShaderCode = new List<string>(1);
            colorPinData.ShaderCode.Add("return %CONNECTED_VARIABLE_NAME%;");

            ShaderPinData positionOffsetPinData = new ShaderPinData("Position Offset", PinType.IN, ShaderVariableType.FLOAT3, ShaderType.VERTEX_SHADER);
            positionOffsetPinData.ShaderCode = new List<string>(1);
            positionOffsetPinData.ShaderCode.Add("input.position += %CONNECTED_VARIABLE_NAME%;");

            pinData.Add(colorPinData);
            pinData.Add(positionOffsetPinData);

            outputNode = CreateNode(new ShaderNodeData("OUTPUT", pinData), new Point(NodesPanel.Width / 2, NodesPanel.Height / 2));
        }

        public void HandleMouseMessageEvents(MouseEventArguments args)
        {
            // Disable checking the radiobutton when double clicking
            if (args.type == MouseEventType.LBUTTON_DOUBLE_CLICK)
            {
                foreach (RadioButton pin in pins)
                {
                    if (pin.ClientRectangle.Contains(pin.PointToClient(args.mousePositon)))
                        args.isHandled = true;
                }

                return;
            }

            // Handling connecting nodes
            if (args.type == MouseEventType.LBUTTON_DOWN)
            {
                foreach (RadioButton pin in pins)
                {
                    if (!pin.Checked && pin.ClientRectangle.Contains(pin.PointToClient(args.mousePositon)))
                    {
                        args.isHandled = true;
                        selectedPin = pin;

                        GetShaderPinData(selectedPin).MousePoint = args.mousePositon;
                        NodesPanel.Refresh();

                        return;
                    }
                }
            }

            if (args.type == MouseEventType.MOUSE_MOTION && selectedPin != null)
            {
                GetShaderPinData(selectedPin).MousePoint = args.mousePositon;
                NodesPanel.Refresh();
            }

            if (args.type == MouseEventType.LBUTTON_UP && selectedPin != null)
            {
                foreach (RadioButton pin in pins)
                {
                    if (!pin.Checked &&
                        GetShaderPinData(pin).Type != GetShaderPinData(selectedPin).Type &&
                        GetShaderPinData(pin).ValueType == GetShaderPinData(selectedPin).ValueType &&
                        pin.ClientRectangle.Contains(pin.PointToClient(args.mousePositon)))
                    {
                        args.isHandled = true;

                        selectedPin.Checked = true;
                        pin.Checked = true;

                        GetShaderPinData(selectedPin).ConnectedPin = pin;
                        GetShaderPinData(selectedPin).MousePoint = Point.Empty;

                        GetShaderPinData(pin).ConnectedPin = selectedPin;
                        GetShaderPinData(pin).MousePoint = Point.Empty;

                        NodesPanel.Refresh();

                        selectedPin = null;
                        return;
                    }
                }

                GetShaderPinData(selectedPin).ConnectedPin = null;
                GetShaderPinData(selectedPin).MousePoint = Point.Empty;
                selectedPin = null;
                NodesPanel.Refresh();

                return;
            }

            // Moving of nodes
            if (args.type == MouseEventType.LBUTTON_DOWN)
            {
                foreach (Control node in NodesPanel.Controls)
                {
                    if (node.ClientRectangle.Contains(node.PointToClient(args.mousePositon)))
                    {
                        isMouseDraggingNode = true;
                        previousMouseLocation = args.mousePositon;
                        movingNode = node;
                        return;
                    }
                }
            }

            if (args.type == MouseEventType.MOUSE_MOTION && isMouseDraggingNode && movingNode != null)
            {
                Point deltaMotion = new Point(args.mousePositon.X - previousMouseLocation.X, args.mousePositon.Y - previousMouseLocation.Y);
                movingNode.Location = new Point(movingNode.Location.X + deltaMotion.X, movingNode.Location.Y + deltaMotion.Y);
                previousMouseLocation = args.mousePositon;
                return;
            }

            if (args.type == MouseEventType.LBUTTON_UP && isMouseDraggingNode)
            {
                isMouseDraggingNode = false;
                return;
            }

            // Creating node
            if (args.type == MouseEventType.LBUTTON_DOWN)
            {
                Queue<TreeNode> treeNodes = new Queue<TreeNode>(NodeSelector.Nodes.OfType<TreeNode>().ToArray());
                while (treeNodes.Count > 0)
                {
                    TreeNode treeNode = treeNodes.Dequeue();
                    if (treeNode.Bounds.Contains(NodeSelector.PointToClient(args.mousePositon)))
                    {
                        if (treeNode.Level == 0)
                            return;

                        creationNode = CreateNode((ShaderNodeData)((ShaderNodeData)treeNode.Tag).Clone(), NodesPanel.PointToClient(args.mousePositon));
                        creationNode.BackColor = Color.LightCyan;
                        previousMouseLocation = args.mousePositon;

                        return;
                    }

                    // Enqueue child tree nodes of the currently being processed tree node
                    foreach (TreeNode childTreeNode in treeNode.Nodes)
                    {
                        treeNodes.Enqueue(childTreeNode);
                    }
                }
            }

            if (args.type == MouseEventType.MOUSE_MOTION && creationNode != null)
            {
                Point deltaMotion = new Point(args.mousePositon.X - previousMouseLocation.X, args.mousePositon.Y - previousMouseLocation.Y);
                creationNode.Location = new Point(creationNode.Location.X + deltaMotion.X, creationNode.Location.Y + deltaMotion.Y);
                previousMouseLocation = args.mousePositon;
                return;
            }

            if (args.type == MouseEventType.LBUTTON_UP && creationNode != null)
            {
                creationNode.BackColor = Color.DarkGray;
                creationNode = null;
                return;
            }
        }

        public void HandleKeyEvent(Keys keys)
        {
            if (keys == Keys.Delete && selectedNode != null && selectedNode != outputNode)
            {
                // Removing connections
                foreach (RadioButton inputPin in GetInputPins(selectedNode))
                {
                    RemoveConnection(inputPin, new MouseEventArgs(MouseButtons.Right, 1, 0, 0, 0));
                }

                foreach (RadioButton outputPin in GetOutputPins(selectedNode))
                {
                    RemoveConnection(outputPin, new MouseEventArgs(MouseButtons.Right, 1, 0, 0, 0));
                }

                // Delete the node
                selectedNode.Parent.Controls.Remove(selectedNode);
                selectedNode = null;
            }
        }

        private FlowLayoutPanel CreateNode(ShaderNodeData shaderNodeData, Point centerPosition)
        {
            List<ShaderPinData> inputPins = shaderNodeData.PinData.FindAll((ShaderPinData pin) => { return pin.Type == PinType.IN; });
            List<ShaderPinData> outputPins = shaderNodeData.PinData.FindAll((ShaderPinData pin) => { return pin.Type == PinType.OUT; });

            FlowLayoutPanel nodePanel = new FlowLayoutPanel();
            nodePanel.BackColor = Color.DarkGray;
            nodePanel.BorderStyle = BorderStyle.None;
            nodePanel.FlowDirection = FlowDirection.TopDown;
            nodePanel.MinimumSize = new Size(208, 0);
            nodePanel.MaximumSize = new Size(208, 0);
            nodePanel.Tag = shaderNodeData;
            nodePanel.Paint += PaintNodePanel;
            nodePanel.AutoSize = true;
            nodePanel.Location = new Point(centerPosition.X - (nodePanel.Size.Width / 2), centerPosition.Y);

            Label titleLabel = new Label();
            titleLabel.Text = shaderNodeData.Title;
            titleLabel.TextAlign = ContentAlignment.MiddleCenter;
            titleLabel.AutoSize = false;
            titleLabel.Margin = new Padding(3);
            titleLabel.Size = new Size(201, 22);
            titleLabel.MouseClick += NodeSelected;
            nodePanel.Controls.Add(titleLabel);

            for (int i = 0; i < Math.Max(inputPins.Count, outputPins.Count); ++i)
            {
                FlowLayoutPanel nodePinsPanel = new FlowLayoutPanel();
                if ((inputPins.Count - 1) >= i)
                    nodePinsPanel.FlowDirection = FlowDirection.LeftToRight;
                else
                    nodePinsPanel.FlowDirection = FlowDirection.RightToLeft;
                nodePinsPanel.MinimumSize = new Size(200, 20);
                nodePinsPanel.MaximumSize = new Size(200, 20);
                nodePinsPanel.Location = new Point(4, 0);
                nodePinsPanel.AutoSize = false;

                if ((inputPins.Count - 1) >= i)
                {
                    ShaderPinData inputPin = inputPins[i];

                    Panel pinRadioPanel = new Panel(); // Panel is needed because otherwise won't be able to have multiple radiobuttons checked
                    pinRadioPanel.AutoSize = true;
                    pinRadioPanel.Margin = new Padding(0);
                    nodePinsPanel.Controls.Add(pinRadioPanel);

                    RadioButton pinRadioButton = new RadioButton();
                    pinRadioButton.RightToLeft = RightToLeft.No;
                    pinRadioButton.Checked = false;
                    pinRadioButton.Text = inputPin.Name;
                    pinRadioButton.AutoSize = false;
                    pinRadioButton.Size = new Size((nodePinsPanel.Size.Width / 2) - pinRadioButton.Margin.Left, nodePinsPanel.Size.Height);
                    pinRadioButton.Tag = inputPin.Clone();
                    pinRadioButton.MouseUp += RemoveConnection;
                    pinRadioPanel.Controls.Add(pinRadioButton);

                    this.pins.Add(pinRadioButton);
                }

                if ((outputPins.Count - 1) >= i)
                {
                    ShaderPinData outputPin = outputPins[i];

                    Panel pinRadioPanel = new Panel(); // Panel is needed because otherwise won't be able to have multiple radiobuttons checked
                    pinRadioPanel.AutoSize = true;
                    pinRadioPanel.Margin = new Padding(0);
                    nodePinsPanel.Controls.Add(pinRadioPanel);

                    RadioButton pinRadioButton = new RadioButton();
                    pinRadioButton.RightToLeft = RightToLeft.Yes;
                    pinRadioButton.Checked = false;
                    pinRadioButton.Text = outputPin.Name;
                    pinRadioButton.AutoSize = false;
                    pinRadioButton.Size = new Size((nodePinsPanel.Size.Width / 2) - pinRadioButton.Margin.Left, nodePinsPanel.Size.Height);
                    pinRadioButton.Tag = outputPin.Clone();
                    pinRadioButton.MouseUp += RemoveConnection;
                    pinRadioPanel.Controls.Add(pinRadioButton);

                    this.pins.Add(pinRadioButton);
                }

                nodePanel.Controls.Add(nodePinsPanel);
            }

            NodesPanel.Controls.Add(nodePanel);
            return nodePanel;
        }

        private void NodesPanelMouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Left)
                return;

            isMouseDraggingPanel = true;
            previousMouseLocation = e.Location;
        }

        private void NodesPanelMouseMove(object sender, MouseEventArgs e)
        {
            if (!isMouseDraggingPanel)
                return;

            Point deltaMousePosition = new Point(e.Location.X - previousMouseLocation.X, e.Location.Y - previousMouseLocation.Y);
            foreach (Control control in NodesPanel.Controls)
            {
                control.Location = new Point(control.Location.X + deltaMousePosition.X, control.Location.Y + deltaMousePosition.Y);
            }

            NodesPanel.Refresh();
            previousMouseLocation = e.Location;
        }

        private void NodesPanelMouseUp(object sender, MouseEventArgs e)
        {
            isMouseDraggingPanel = false;
        }

        private void RemoveConnection(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Right)
                return;

            RadioButton pin = (RadioButton)sender;
            RadioButton connectedPin = GetShaderPinData(pin).ConnectedPin;

            if (connectedPin != null)
            {
                pin.Checked = false;
                connectedPin.Checked = false;

                GetShaderPinData(pin).ConnectedPin = null;
                GetShaderPinData(connectedPin).ConnectedPin = null;

                NodesPanel.Refresh();
            }
        }

        private void NodeSelected(object sender, EventArgs e)
        {
            // Red border around the selected node
            Control previouslySelectedNode = selectedNode;
            selectedNode = ((Control)sender).Parent;

            if (previouslySelectedNode != null)
                previouslySelectedNode.Refresh();
            selectedNode.Refresh();

            // Update properties
            ShaderNodeData nodeData = (ShaderNodeData)selectedNode.Tag;
            List<CustomProperty> properties = nodeData.Properties;

            CustomObjectType propertiesObject = new CustomObjectType
            {
                Name = nodeData.Title,
                UniqueName = nodeData.UniqueTitle,
                Properties = properties
            };

            NodeProperties.SelectedObject = propertiesObject;
        }

        private void PaintNodePanel(object sender, PaintEventArgs e)
        {
            Control control = (Control)sender;

            Color color = Color.Black;
            if (control == selectedNode)
                color = Color.Red;

            e.Graphics.DrawRectangle(new Pen(color), new Rectangle(0, 0, control.Width - 1, control.Height - 1));
        }

        private void PaintConnectionLines(object sender, PaintEventArgs e)
        {
            foreach (RadioButton control in pins)
            {
                RadioButton connectedControl = GetShaderPinData(control).ConnectedPin;
                Point mousePoint = GetShaderPinData(control).MousePoint;
                if (connectedControl != null)
                {
                    RadioButton inputPin = GetShaderPinData(control).Type == PinType.IN ? control : connectedControl;
                    RadioButton outputPin = (inputPin == control) ? connectedControl : control;

                    Point input = NodesPanel.PointToClient(inputPin.PointToScreen(Point.Add(inputPin.Location, new Size(inputPin.RightToLeft == RightToLeft.Yes ? inputPin.Width : 0, inputPin.Height / 2))));
                    Point output = NodesPanel.PointToClient(outputPin.PointToScreen(Point.Add(outputPin.Location, new Size(outputPin.RightToLeft == RightToLeft.Yes ? outputPin.Width : 0, outputPin.Height / 2))));

                    DrawConnection(e.Graphics, input, output);
                }
                else if (mousePoint != Point.Empty)
                {
                    Point input = NodesPanel.PointToClient(control.PointToScreen(Point.Add(control.Location, new Size(control.RightToLeft == RightToLeft.Yes ? control.Width : 0, control.Height / 2))));
                    Point output = NodesPanel.PointToClient(mousePoint);

                    if (GetShaderPinData(control).Type == PinType.OUT)
                    {
                        Point temp = input;
                        input = output;
                        output = temp;
                    }

                    DrawConnection(e.Graphics, input, output);
                }
            }
        }

        // https://github.com/komorra/NodeEditorWinforms/blob/master/NodeEditor/NodesGraph.cs
        // https://github.com/komorra/NodeEditorWinforms/blob/master/License.md
        private void DrawConnection(Graphics g, Point input, Point output)
        {
            g.InterpolationMode = InterpolationMode.HighQualityBilinear;
            g.SmoothingMode = SmoothingMode.HighQuality;

            const int interpolation = 48;

            PointF[] points = new PointF[interpolation];
            for (int i = 0; i < interpolation; i++)
            {
                float amount = i / (float)(interpolation - 1);

                var lx = Lerp(output.X, input.X, amount);
                var d = Math.Min(Math.Abs(input.X - output.X), 100);
                var a = new PointF((float)Scale(amount, 0, 1, output.X, output.X + d),
                    output.Y);
                var b = new PointF((float)Scale(amount, 0, 1, input.X - d, input.X), input.Y);

                var bas = Sat(Scale(amount, 0.1, 0.9, 0, 1));
                var cos = Math.Cos(bas * Math.PI);
                if (cos < 0)
                {
                    cos = -Math.Pow(-cos, 0.2);
                }
                else
                {
                    cos = Math.Pow(cos, 0.2);
                }
                amount = (float)cos * -0.5f + 0.5f;

                var f = Lerp(a, b, amount);
                points[i] = f;
            }

            g.DrawLines(new Pen(Color.Red), points);
        }

        // https://github.com/komorra/NodeEditorWinforms/blob/master/NodeEditor/NodesGraph.cs
        // https://github.com/komorra/NodeEditorWinforms/blob/master/License.md
        private double Sat(double x)
        {
            if (x < 0) return 0;
            if (x > 1) return 1;
            return x;
        }

        // https://github.com/komorra/NodeEditorWinforms/blob/master/NodeEditor/NodesGraph.cs
        // https://github.com/komorra/NodeEditorWinforms/blob/master/License.md
        private double Scale(double x, double a, double b, double c, double d)
        {
            double s = (x - a) / (b - a);
            return s * (d - c) + c;
        }

        // https://github.com/komorra/NodeEditorWinforms/blob/master/NodeEditor/NodesGraph.cs
        // https://github.com/komorra/NodeEditorWinforms/blob/master/License.md
        private float Lerp(float a, float b, float amount)
        {
            return a * (1f - amount) + b * amount;
        }

        // https://github.com/komorra/NodeEditorWinforms/blob/master/NodeEditor/NodesGraph.cs
        // https://github.com/komorra/NodeEditorWinforms/blob/master/License.md
        private PointF Lerp(PointF a, PointF b, float amount)
        {
            PointF result = new PointF();

            result.X = a.X * (1f - amount) + b.X * amount;
            result.Y = a.Y * (1f - amount) + b.Y * amount;

            return result;
        }

        private ShaderPinData GetShaderPinData(RadioButton pin)
        {
            return (ShaderPinData)pin.Tag;
        }

        private ShaderNodeData GetShaderNodeData(Control node)
        {
            return (ShaderNodeData)node.Tag;
        }

        public void Compile()
        {
            if (ShaderNameTextBox.Text.Length == 0)
            {
                MessageBox.Show("Please enter a shader name", "Empty name field", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Getting the beginning nodes
            List<Control> firstNodes = new List<Control>();
            GetFirstNodes(GetShaderNodeData(outputNode), ref firstNodes);
            //GetFirstNodes(colorPinData, ref firstNodes);

            // Initializing engine shader data
            uint shaderID = Bridge.CreateShaderData(ShaderNameTextBox.Text, @"C:\Users\Administrator\Perforce\STEFF-DESKTOP\Personal\Engine\Dev\Resources\Shaders\" + ShaderNameTextBox.Text);

            // Getting the shader data
            string vertexShaderGlobalVariables;
            string pixelShaderGlobalVariables;
            string vertexShaderCode;
            string pixelShaderCode;
            GetShaderData(firstNodes, shaderID, out vertexShaderGlobalVariables, out pixelShaderGlobalVariables, out vertexShaderCode, out pixelShaderCode);

            // Writing the vertex shader
            string vertexShaderPath = @"C:\Users\Administrator\Perforce\STEFF-DESKTOP\Personal\Engine\Dev\Resources\Shaders\" + ShaderNameTextBox.Text + "-VertexShader.hlsl";
            using (StreamWriter shaderFile = new StreamWriter(vertexShaderPath))
            {
                // Structs
                shaderFile.WriteLine("struct INPUT_VERTEX");
                shaderFile.WriteLine("{");
                shaderFile.WriteLine("\tfloat3 position : POSITION;");
                shaderFile.WriteLine("\tfloat2 texCoord : TEXCOORD0;");
                shaderFile.WriteLine("};\n");

                shaderFile.WriteLine("struct OUTPUT_VERTEX");
                shaderFile.WriteLine("{");
                shaderFile.WriteLine("\tfloat4 position : SV_POSITION;");
                shaderFile.WriteLine("\tfloat3 worldPosition : WORLD_POSITION;");
                shaderFile.WriteLine("\tfloat2 texCoord : TEXCOORD0;");
                shaderFile.WriteLine("};\n");

                // Global variables
                shaderFile.WriteLine("cbuffer MeshConstantBuffer : register(b0)");
                shaderFile.WriteLine("{");
                shaderFile.WriteLine("\tmatrix g_WorldMatrix;");
                shaderFile.WriteLine("};\n");

                shaderFile.WriteLine("cbuffer CameraConstantBuffer : register(b1)");
                shaderFile.WriteLine("{");
                shaderFile.WriteLine("\tmatrix g_ViewProjectionMatrix;");
                shaderFile.WriteLine("};\n");

                shaderFile.WriteLine("SamplerState g_Sampler : register(s0);\n");
                shaderFile.WriteLine(vertexShaderGlobalVariables);

                // Vertex shader body
                shaderFile.WriteLine("OUTPUT_VERTEX main(INPUT_VERTEX input)");
                shaderFile.WriteLine("{");
                shaderFile.WriteLine(vertexShaderCode);
                shaderFile.WriteLine("\tOUTPUT_VERTEX output;");
                shaderFile.WriteLine("\toutput.position = mul(mul(float4(input.position, 1.f), g_WorldMatrix), g_ViewProjectionMatrix);");
                shaderFile.WriteLine("\toutput.worldPosition = input.position;");
                shaderFile.WriteLine("\toutput.texCoord = input.texCoord;");
                shaderFile.WriteLine("\n\treturn output;");
                shaderFile.WriteLine("}");
            }

            // Writing the pixel shader
            string pixelShaderPath = @"C:\Users\Administrator\Perforce\STEFF-DESKTOP\Personal\Engine\Dev\Resources\Shaders\" + ShaderNameTextBox.Text + "-PixelShader.hlsl";
            using (StreamWriter shaderFile = new StreamWriter(pixelShaderPath))
            {
                // Structs
                shaderFile.WriteLine("struct OUTPUT_VERTEX");
                shaderFile.WriteLine("{");
                shaderFile.WriteLine("\tfloat4 position : SV_POSITION;");
                shaderFile.WriteLine("\tfloat3 worldPosition : WORLD_POSITION;");
                shaderFile.WriteLine("\tfloat2 texCoord : TEXCOORD0;");
                shaderFile.WriteLine("};\n");

                // Global variables
                shaderFile.WriteLine("SamplerState g_Sampler : register(s0);");
                shaderFile.WriteLine(pixelShaderGlobalVariables);

                // Pixel shader body
                shaderFile.WriteLine("float4 main(OUTPUT_VERTEX input) : SV_TARGET");
                shaderFile.WriteLine("{");
                shaderFile.WriteLine(pixelShaderCode);
                shaderFile.WriteLine("}");
            }

            // Compiling the vertex shader
            string errorMessage = Bridge.CompileShader(vertexShaderPath, ShaderType.VERTEX_SHADER);
            if (errorMessage != string.Empty)
                MessageBox.Show(errorMessage, "Vertex shader compilation failed", MessageBoxButtons.OK, MessageBoxIcon.Error);

            // Compiling the pixel shader
            errorMessage = Bridge.CompileShader(pixelShaderPath, ShaderType.PIXEL_SHADER);
            if (errorMessage != string.Empty)
                MessageBox.Show(errorMessage, "Pixel shader compilation failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void GetFirstNodes(ShaderNodeData nodeData, ref List<Control> firstNodes)
        {
            foreach (RadioButton pin in ControlHelper.GetChildrenOfType<RadioButton>(outputNode))
            {
                GetFirstNodes(GetShaderPinData(pin), ref firstNodes);
            }
        }

        private void GetFirstNodes(ShaderPinData pinData, ref List<Control> firstNodes)
        {
            if (pinData.ConnectedPin == null)
                return;

            GetShaderPinData(pinData.ConnectedPin).ShaderTypes.AddRange(pinData.ShaderTypes);
            Control node = pinData.ConnectedPin.Parent.Parent.Parent;

            bool isEndNode = true;
            foreach (RadioButton inputPin in GetInputPins(node))
            {
                ShaderPinData inputPinData = GetShaderPinData(inputPin);
                inputPinData.ShaderTypes = pinData.ShaderTypes;
                if (inputPinData.ConnectedPin != null)
                {
                    isEndNode = false;
                    GetFirstNodes(GetShaderPinData(inputPin), ref firstNodes);
                }
            }

            if (isEndNode && !firstNodes.Contains(node))
                firstNodes.Add(node);
        }

        private static Regex inputVariableNameRegex = new Regex(@"%INPUT_VARIABLE_NAME_([^%]+)%", RegexOptions.Compiled);
        private static Regex inputGlobalVariableNameRegex = new Regex(@"%INPUT_GLOBAL_VARIABLE_NAME_([^%]+)%", RegexOptions.Compiled);
        private static Regex propertyVariableRegex = new Regex(@"%PROPERTY_VALUE_([^%]+)%", RegexOptions.Compiled);
        private void GetShaderData(List<Control> firstNodes, uint shaderID, out string vertexShaderGlobalVariables, out string pixelShaderGlobalVariables, out string vertexShaderCode, out string pixelShaderCode)
        {
            StringBuilder vertexShaderGlobalVariablesStringBuilder = new StringBuilder();
            StringBuilder pixelShaderGlobalVariablesStringBuilder = new StringBuilder();
            StringBuilder vertexShaderCodeStringBuilder = new StringBuilder();
            StringBuilder pixelShaderCodeStringBuilder = new StringBuilder();

            List<Control> processedNodes = new List<Control>();

            Queue<Control> nodeQueue = new Queue<Control>(firstNodes);
            while (nodeQueue.Count > 0)
            {
                Control node = nodeQueue.Dequeue();
                ShaderNodeData nodeData = (ShaderNodeData)node.Tag;

                if (nodeData.IsProcessed) // Already processed, don't process again
                    continue;

                // Send properties to engine shader data
                if (nodeData.Properties != null)
                {
                    for (int i = 0; i < nodeData.Properties.Count; ++i)
                    {
                        CustomProperty property = nodeData.Properties[i];
                        if (!property.ReadOnly && property.Type == typeof(String))
                        {
                            // Get ShaderTypes of all output
                            List<ShaderType> nodeShaderTypes = new List<ShaderType>();
                            foreach (RadioButton outputPin in GetOutputPins(node))
                            {
                                nodeShaderTypes.AddRange(GetShaderPinData(outputPin).ShaderTypes);
                            }

                            foreach (ShaderType shaderType in nodeShaderTypes.Distinct())
                            {
                                Bridge.AddShaderData(shaderID, (String)(property.Value ?? property.DefaultValue), shaderType);
                            }
                        }
                    }
                }

                // Input pins
                bool hasUnprocessedInput = false;
                foreach (RadioButton pin in GetInputPins(node))
                {
                    ShaderPinData pinData = GetShaderPinData(pin);

                    if (pinData.ConnectedPin != null)
                    {
                        // Setting the input pins variable names to the same as the connected output pin
                        pinData.SetVariableID(VariableScope.GLOBAL, GetShaderPinData(pinData.ConnectedPin).GetVariableID(VariableScope.GLOBAL));
                        pinData.SetVariableID(VariableScope.SHADER, GetShaderPinData(pinData.ConnectedPin).GetVariableID(VariableScope.SHADER));

                        // Check if the node connected to the input pin is already processed
                        if (!GetShaderNodeData(pinData.ConnectedPin.Parent.Parent.Parent).IsProcessed)
                        {
                            hasUnprocessedInput = true;
                            break;
                        }

                        // Shader code (output node contains input pins which need pixel shader code)
                        if (pinData.ShaderCode != null)
                        {
                            foreach (string shaderCodeSnippet in pinData.ShaderCode)
                            {
                                // Replacing the placeholders
                                string convertedShaderCodeSnippet = shaderCodeSnippet;
                                if (pinData.ConnectedPin != null)
                                {
                                    convertedShaderCodeSnippet = convertedShaderCodeSnippet.Replace("%CONNECTED_GLOBAL_VARIABLE_NAME%", GetShaderPinData(pinData.ConnectedPin).GetVariableName(VariableScope.GLOBAL));
                                    convertedShaderCodeSnippet = convertedShaderCodeSnippet.Replace("%CONNECTED_VARIABLE_NAME%", GetShaderPinData(pinData.ConnectedPin).GetVariableName(VariableScope.SHADER));
                                }

                                // Adding the code snippet to the string builder
                                if (pinData.ShaderTypes.Contains(ShaderType.VERTEX_SHADER))
                                    vertexShaderCodeStringBuilder.AppendLine(convertedShaderCodeSnippet);
                                if (pinData.ShaderTypes.Contains(ShaderType.PIXEL_SHADER))
                                    pixelShaderCodeStringBuilder.AppendLine(convertedShaderCodeSnippet);
                            }
                        }
                    }
                }

                // Some of the input pins connect to nodes which are not processed yet - wait until these nodes are processed
                if (hasUnprocessedInput)
                {
                    nodeQueue.Enqueue(node);
                    continue;
                }

                // Output pins
                foreach (RadioButton pin in GetOutputPins(node))
                {
                    ShaderPinData pinData = GetShaderPinData(pin);

                    // Global variable
                    if (pinData.GlobalVariable != null)
                    {
                        string convertedGlobalVariable = pinData.GlobalVariable.Replace("%VARIABLE_NAME%", pinData.GetVariableName(VariableScope.GLOBAL));

                        if (pinData.ShaderTypes.Contains(ShaderType.VERTEX_SHADER))
                            vertexShaderGlobalVariablesStringBuilder.AppendLine(convertedGlobalVariable);
                        if (pinData.ShaderTypes.Contains(ShaderType.PIXEL_SHADER))
                            pixelShaderGlobalVariablesStringBuilder.AppendLine(convertedGlobalVariable);
                    }

                    // Shader code
                    if (pinData.ShaderCode != null)
                    {
                        foreach (string shaderCodeSnippet in pinData.ShaderCode)
                        {
                            // Replacing the placeholders
                            string convertedShaderCodeSnippet = shaderCodeSnippet.Replace("%VARIABLE_NAME%", pinData.GetVariableName(VariableScope.SHADER));
                            convertedShaderCodeSnippet = convertedShaderCodeSnippet.Replace("%GLOBAL_VARIABLE_NAME%", pinData.GetVariableName(VariableScope.GLOBAL));

                            if (pinData.ConnectedPin != null)
                            {
                                convertedShaderCodeSnippet = convertedShaderCodeSnippet.Replace("%CONNECTED_VARIABLE_NAME%", GetShaderPinData(pinData.ConnectedPin).GetVariableName(VariableScope.SHADER));
                                convertedShaderCodeSnippet = convertedShaderCodeSnippet.Replace("%CONNECTED_GLOBAL_VARIABLE_NAME%", GetShaderPinData(pinData.ConnectedPin).GetVariableName(VariableScope.GLOBAL));
                            }

                            // Replace input variable placeholders with the actual variable
                            convertedShaderCodeSnippet = ReplaceInputVariablePlaceholder(pin, inputVariableNameRegex, convertedShaderCodeSnippet, VariableScope.SHADER);
                            convertedShaderCodeSnippet = ReplaceInputVariablePlaceholder(pin, inputGlobalVariableNameRegex, convertedShaderCodeSnippet, VariableScope.GLOBAL);

                            // Replace property placeholders
                            convertedShaderCodeSnippet = ReplacePropertyVariablePlaceholder(nodeData, propertyVariableRegex, convertedShaderCodeSnippet);

                            // Replace special placeholders
                            convertedShaderCodeSnippet = convertedShaderCodeSnippet.Replace("%TEXCOORD%", "input.texCoord");
                            convertedShaderCodeSnippet = convertedShaderCodeSnippet.Replace("%SAMPLER%", "g_Sampler");

                            // Adding the code snippet to the string builder
                            if (pinData.ShaderTypes.Contains(ShaderType.VERTEX_SHADER))
                                vertexShaderCodeStringBuilder.AppendLine(convertedShaderCodeSnippet);
                            if (pinData.ShaderTypes.Contains(ShaderType.PIXEL_SHADER))
                                pixelShaderCodeStringBuilder.AppendLine(convertedShaderCodeSnippet);
                        }
                    }

                    // Add the nodes which are connected to the output pins to the queue
                    if (pinData.ConnectedPin != null)
                    {
                        Control connectedNode = pinData.ConnectedPin.Parent.Parent.Parent;
                        if (!nodeQueue.Contains(connectedNode))
                            nodeQueue.Enqueue(connectedNode);
                    }

                    pinData.ShaderTypes = new List<ShaderType>(); // Reset shader types (for next compilation)

                    GetShaderNodeData(node).IsProcessed = true;
                    processedNodes.Add(node);
                }
            }

            // Reset IsProcessed variable for next output node / compilation
            foreach (Control node in processedNodes)
            {
                GetShaderNodeData(node).IsProcessed = false;
            }

            vertexShaderGlobalVariables = vertexShaderGlobalVariablesStringBuilder.ToString();
            pixelShaderGlobalVariables = pixelShaderGlobalVariablesStringBuilder.ToString();
            vertexShaderCode = vertexShaderCodeStringBuilder.ToString();
            pixelShaderCode = pixelShaderCodeStringBuilder.ToString();
        }

        private string ReplaceInputVariablePlaceholder(RadioButton pin, Regex regex, string line, VariableScope type)
        {
            foreach (Match match in regex.Matches(line))
            {
                if (match.Success && match.Groups.Count >= 2 && match.Groups[1].Captures.Count == 1)
                {
                    foreach (RadioButton inputPin in GetInputPins(pin.Parent.Parent.Parent))
                    {
                        ShaderPinData inputPinData = GetShaderPinData(inputPin);
                        if (inputPinData.Name == match.Groups[1].Captures[0].Value)
                            line = regex.Replace(line, inputPinData.GetVariableName(type), 1);
                    }
                }
            }

            return line;
        }

        private string ReplacePropertyVariablePlaceholder(ShaderNodeData nodeData, Regex regex, string line)
        {
            if (nodeData.Properties == null || nodeData.Properties.Count == 0)
                return line;

            foreach (Match match in regex.Matches(line))
            {
                if (match.Success && match.Groups.Count >= 2 && match.Groups[1].Captures.Count == 1)
                {
                    foreach (CustomProperty property in nodeData.Properties)
                    {
                        if (property.PlaceholderName == match.Groups[1].Captures[0].Value)
                            line = regex.Replace(line, (property.Value ?? property.DefaultValue).ToString(), 1);
                    }
                }
            }

            return line;
        }

        private List<RadioButton> GetInputPins(Control node)
        {
            List<RadioButton> inputPinData = new List<RadioButton>();
            foreach (RadioButton pin in ControlHelper.GetChildrenOfType<RadioButton>(node))
            {
                ShaderPinData pinData = GetShaderPinData(pin);
                if (pinData.Type == PinType.IN)
                    inputPinData.Add(pin);
            }

            return inputPinData;
        }

        private List<RadioButton> GetOutputPins(Control node)
        {
            List<RadioButton> outputPinData = new List<RadioButton>();
            foreach (RadioButton pin in ControlHelper.GetChildrenOfType<RadioButton>(node))
            {
                ShaderPinData pinData = GetShaderPinData(pin);
                if (pinData.Type == PinType.OUT)
                    outputPinData.Add(pin);
            }

            return outputPinData;
        }
    }
}
