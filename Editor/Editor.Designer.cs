using System;
using System.Drawing;
using System.Windows;
using System.Windows.Forms;

namespace Editor
{
    partial class EngineEditor
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            //this.RenderPanel = new System.Windows.Forms.Panel();
            this.RenderPanel = new DirectXPanel();
            this.SteffEngineLabel = new System.Windows.Forms.Label();
            this.ComponentNamesList = new System.Windows.Forms.ListBox();
            this.AddComponentButton = new System.Windows.Forms.Button();
            this.PropertiesPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.GameObjectNamesList = new System.Windows.Forms.ListBox();
            this.GameObjectsBox = new System.Windows.Forms.GroupBox();
            this.CreateGameObjectButton = new System.Windows.Forms.Button();
            this.DebugStateButton = new System.Windows.Forms.Button();
            this.TopMenuStrip = new System.Windows.Forms.MenuStrip();
            this.FileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ShaderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenShaderEditorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.CompileShaderEditorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.StandaloneShaderEditorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ExplorerGroupBox = new System.Windows.Forms.GroupBox();
            this.ExplorerFlowLayout = new System.Windows.Forms.FlowLayoutPanel();
            this.ShaderEditor = new ShaderEditor();
            this.RenderPanel.SuspendLayout();
            this.GameObjectsBox.SuspendLayout();
            this.TopMenuStrip.SuspendLayout();
            this.ExplorerGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // RenderPanel
            // 
            this.RenderPanel.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.RenderPanel.Controls.Add(this.SteffEngineLabel);
            this.RenderPanel.Location = new System.Drawing.Point(238, 26);
            this.RenderPanel.Margin = new System.Windows.Forms.Padding(2);
            this.RenderPanel.Name = "RenderPanel";
            this.RenderPanel.Size = new System.Drawing.Size(571, 557);
            this.RenderPanel.TabIndex = 0;
            this.RenderPanel.Enter += new System.EventHandler(this.RenderPanelGotFocus);
            this.RenderPanel.Leave += new System.EventHandler(this.RenderPanelLostFocus);
            this.RenderPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
            this.RenderPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
            // 
            // SteffEngineLabel
            // 
            this.SteffEngineLabel.AutoSize = true;
            this.SteffEngineLabel.Location = new System.Drawing.Point(0, 0);
            this.SteffEngineLabel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.SteffEngineLabel.Name = "SteffEngineLabel";
            this.SteffEngineLabel.Size = new System.Drawing.Size(65, 13);
            this.SteffEngineLabel.TabIndex = 1;
            this.SteffEngineLabel.Text = "Steff Engine";
            // 
            // ComponentNamesList
            // 
            this.ComponentNamesList.FormattingEnabled = true;
            this.ComponentNamesList.Location = new System.Drawing.Point(813, 78);
            this.ComponentNamesList.Name = "ComponentNamesList";
            this.ComponentNamesList.Size = new System.Drawing.Size(227, 459);
            this.ComponentNamesList.TabIndex = 2;
            // 
            // AddComponentButton
            // 
            this.AddComponentButton.Location = new System.Drawing.Point(813, 543);
            this.AddComponentButton.Margin = new System.Windows.Forms.Padding(2);
            this.AddComponentButton.Name = "AddComponentButton";
            this.AddComponentButton.Size = new System.Drawing.Size(227, 41);
            this.AddComponentButton.TabIndex = 3;
            this.AddComponentButton.Text = "Add Component";
            this.AddComponentButton.UseVisualStyleBackColor = true;
            this.AddComponentButton.Click += new System.EventHandler(this.OnAddComponentClick);
            // 
            // PropertiesPanel
            // 
            this.PropertiesPanel.AutoScroll = true;
            this.PropertiesPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PropertiesPanel.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.PropertiesPanel.Location = new System.Drawing.Point(1046, 26);
            this.PropertiesPanel.Name = "PropertiesPanel";
            this.PropertiesPanel.Size = new System.Drawing.Size(364, 557);
            this.PropertiesPanel.TabIndex = 6;
            this.PropertiesPanel.WrapContents = false;
            // 
            // GameObjectNamesList
            // 
            this.GameObjectNamesList.FormattingEnabled = true;
            this.GameObjectNamesList.Location = new System.Drawing.Point(4, 19);
            this.GameObjectNamesList.Name = "GameObjectNamesList";
            this.GameObjectNamesList.Size = new System.Drawing.Size(210, 485);
            this.GameObjectNamesList.TabIndex = 7;
            this.GameObjectNamesList.SelectedIndexChanged += new System.EventHandler(this.GameObjectSelectionChanged);
            this.GameObjectNamesList.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.OnGameObjectsDoubleClick);
            // 
            // GameObjectsBox
            // 
            this.GameObjectsBox.Controls.Add(this.GameObjectNamesList);
            this.GameObjectsBox.Location = new System.Drawing.Point(12, 27);
            this.GameObjectsBox.Name = "GameObjectsBox";
            this.GameObjectsBox.Size = new System.Drawing.Size(220, 510);
            this.GameObjectsBox.TabIndex = 8;
            this.GameObjectsBox.TabStop = false;
            this.GameObjectsBox.Text = "Game Objects";
            // 
            // CreateGameObjectButton
            // 
            this.CreateGameObjectButton.Location = new System.Drawing.Point(12, 543);
            this.CreateGameObjectButton.Margin = new System.Windows.Forms.Padding(2);
            this.CreateGameObjectButton.Name = "CreateGameObjectButton";
            this.CreateGameObjectButton.Size = new System.Drawing.Size(220, 41);
            this.CreateGameObjectButton.TabIndex = 9;
            this.CreateGameObjectButton.Text = "Create GameObject";
            this.CreateGameObjectButton.UseVisualStyleBackColor = true;
            this.CreateGameObjectButton.Click += new System.EventHandler(this.OnCreateGameObjectClick);
            // 
            // DebugStateButton
            // 
            this.DebugStateButton.Location = new System.Drawing.Point(813, 26);
            this.DebugStateButton.Margin = new System.Windows.Forms.Padding(2);
            this.DebugStateButton.Name = "DebugStateButton";
            this.DebugStateButton.Size = new System.Drawing.Size(227, 41);
            this.DebugStateButton.TabIndex = 1;
            this.DebugStateButton.Text = "TOGGLE DEBUG TOOLS";
            this.DebugStateButton.UseVisualStyleBackColor = true;
            this.DebugStateButton.Click += new System.EventHandler(this.OnDebugStateButtonClick);
            // 
            // TopMenuStrip
            // 
            this.TopMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileToolStripMenuItem,
            this.ShaderToolStripMenuItem});
            this.TopMenuStrip.Location = new System.Drawing.Point(0, 0);
            this.TopMenuStrip.Name = "TopMenuStrip";
            this.TopMenuStrip.Size = new System.Drawing.Size(1426, 24);
            this.TopMenuStrip.TabIndex = 10;
            this.TopMenuStrip.Text = "menuStrip1";
            // 
            // FileToolStripMenuItem
            // 
            this.FileToolStripMenuItem.Name = "FileToolStripMenuItem";
            this.FileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.FileToolStripMenuItem.Text = "File";
            // 
            // ShaderToolStripMenuItem
            // 
            this.ShaderToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.OpenShaderEditorToolStripMenuItem,
            this.StandaloneShaderEditorToolStripMenuItem,
            this.CompileShaderEditorToolStripMenuItem});
            this.ShaderToolStripMenuItem.Name = "ShaderToolStripMenuItem";
            this.ShaderToolStripMenuItem.Size = new System.Drawing.Size(60, 20);
            this.ShaderToolStripMenuItem.Text = "Shaders";
            // 
            // OpenShaderEditorToolStripMenuItem
            // 
            this.OpenShaderEditorToolStripMenuItem.Name = "OpenShaderEditorToolStripMenuItem";
            this.OpenShaderEditorToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.OpenShaderEditorToolStripMenuItem.Text = "Toggle Editor";
            this.OpenShaderEditorToolStripMenuItem.Click += new System.EventHandler(this.OpenShaderEditorToolStripMenuItem_Click);
            // 
            // CompileShaderEditorToolStripMenuItem
            // 
            this.CompileShaderEditorToolStripMenuItem.Name = "CompileShaderEditorToolStripMenuItem";
            this.CompileShaderEditorToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.CompileShaderEditorToolStripMenuItem.Text = "Compile Shader";
            this.CompileShaderEditorToolStripMenuItem.Click += new System.EventHandler(this.CompileShaderEditorToolStripMenuItem_Click);
            // 
            // CompileShaderEditorToolStripMenuItem
            // 
            this.StandaloneShaderEditorToolStripMenuItem.Name = "StandaloneShaderEditorToolStripMenuItem";
            this.StandaloneShaderEditorToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.StandaloneShaderEditorToolStripMenuItem.Text = "Standalone";
            this.StandaloneShaderEditorToolStripMenuItem.Click += new System.EventHandler(this.StandaloneShaderEditorToolStripMenuItem_Click);
            // 
            // ExplorerGroupBox
            // 
            this.ExplorerGroupBox.Controls.Add(this.ExplorerFlowLayout);
            this.ExplorerGroupBox.Location = new System.Drawing.Point(12, 589);
            this.ExplorerGroupBox.Name = "ExplorerGroupBox";
            this.ExplorerGroupBox.Size = new System.Drawing.Size(1398, 198);
            this.ExplorerGroupBox.TabIndex = 11;
            this.ExplorerGroupBox.TabStop = false;
            this.ExplorerGroupBox.Text = "Explorer";
            // 
            // ExplorerFlowLayout
            // 
            this.ExplorerFlowLayout.AutoScroll = true;
            this.ExplorerFlowLayout.Location = new System.Drawing.Point(6, 19);
            this.ExplorerFlowLayout.Name = "ExplorerFlowLayout";
            this.ExplorerFlowLayout.Size = new System.Drawing.Size(1386, 173);
            this.ExplorerFlowLayout.TabIndex = 0;
            this.ExplorerFlowLayout.AllowDrop = true;
            this.ExplorerFlowLayout.DragEnter += new System.Windows.Forms.DragEventHandler(this.ExplorerDragEnter);
            this.ExplorerFlowLayout.DragDrop += new System.Windows.Forms.DragEventHandler(this.ExplorerDragDrop);
            this.ExplorerFlowLayout.MouseClick += new System.Windows.Forms.MouseEventHandler(this.ExplorerMouseClick);
            // 
            // ShaderEditor
            // 
            this.ShaderEditor.Location = new System.Drawing.Point(12, 26);
            this.ShaderEditor.Name = "ShaderEditor";
            this.ShaderEditor.Size = new System.Drawing.Size(1406, 761);
            this.ShaderEditor.TabIndex = 11;
            this.ShaderEditor.Visible = false;
            // 
            // EngineEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1426, 799);
            this.Controls.Add(this.ShaderEditor);
            this.Controls.Add(this.ExplorerGroupBox);
            this.Controls.Add(this.GameObjectsBox);
            this.Controls.Add(this.CreateGameObjectButton);
            this.Controls.Add(this.RenderPanel);
            this.Controls.Add(this.AddComponentButton);
            this.Controls.Add(this.DebugStateButton);
            this.Controls.Add(this.ComponentNamesList);
            this.Controls.Add(this.PropertiesPanel);
            this.Controls.Add(this.TopMenuStrip);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.KeyPreview = true;
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "EngineEditor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Engine Editor";
            this.Load += new System.EventHandler(this.OnWindowLoaded);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.OnKeyUp);
            this.RenderPanel.ResumeLayout(false);
            this.RenderPanel.PerformLayout();
            this.GameObjectsBox.ResumeLayout(false);
            this.TopMenuStrip.ResumeLayout(false);
            this.TopMenuStrip.PerformLayout();
            this.ExplorerGroupBox.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private DirectXPanel RenderPanel;
        //private Panel RenderPanel;
        private Label SteffEngineLabel;
        private ListBox ComponentNamesList;
        private Button AddComponentButton;
        private FlowLayoutPanel PropertiesPanel;
        private ListBox GameObjectNamesList;
        private GroupBox GameObjectsBox;
        private Button CreateGameObjectButton;
        private Button DebugStateButton;
        private MenuStrip TopMenuStrip;
        private ToolStripMenuItem FileToolStripMenuItem;
        private ToolStripMenuItem ShaderToolStripMenuItem;
        private ToolStripMenuItem OpenShaderEditorToolStripMenuItem;
        private ToolStripMenuItem CompileShaderEditorToolStripMenuItem;
        private ToolStripMenuItem StandaloneShaderEditorToolStripMenuItem;
        private GroupBox ExplorerGroupBox;
        private FlowLayoutPanel ExplorerFlowLayout;
        private ShaderEditor ShaderEditor;
    }
}

