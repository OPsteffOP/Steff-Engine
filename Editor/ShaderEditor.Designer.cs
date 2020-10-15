namespace Editor
{
    partial class ShaderEditor
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.NodesPanel = new System.Windows.Forms.Panel();
            this.NodeProperties = new System.Windows.Forms.PropertyGrid();
            this.NodeSelectorBox = new System.Windows.Forms.GroupBox();
            this.ShaderNameTextBox = new System.Windows.Forms.TextBox();
            this.ShaderNameLabel = new System.Windows.Forms.Label();
            this.NodeSelector = new System.Windows.Forms.TreeView();
            this.NodeSelectorBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // NodesPanel
            // 
            this.NodesPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.NodesPanel.Location = new System.Drawing.Point(266, 3);
            this.NodesPanel.Name = "NodesPanel";
            this.NodesPanel.Size = new System.Drawing.Size(907, 755);
            this.NodesPanel.TabIndex = 1;
            this.NodesPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.NodesPanelMouseDown);
            this.NodesPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.NodesPanelMouseMove);
            this.NodesPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.NodesPanelMouseUp);
            // 
            // NodeProperties
            // 
            this.NodeProperties.Location = new System.Drawing.Point(3, 32);
            this.NodeProperties.Name = "NodeProperties";
            this.NodeProperties.Size = new System.Drawing.Size(257, 726);
            this.NodeProperties.TabIndex = 2;
            // 
            // NodeSelectorBox
            // 
            this.NodeSelectorBox.Controls.Add(this.NodeSelector);
            this.NodeSelectorBox.Location = new System.Drawing.Point(1179, 3);
            this.NodeSelectorBox.Name = "NodeSelectorBox";
            this.NodeSelectorBox.Size = new System.Drawing.Size(224, 755);
            this.NodeSelectorBox.TabIndex = 3;
            this.NodeSelectorBox.TabStop = false;
            this.NodeSelectorBox.Text = "Nodes";
            // 
            // ShaderNameTextBox
            // 
            this.ShaderNameTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.ShaderNameTextBox.Location = new System.Drawing.Point(58, 5);
            this.ShaderNameTextBox.Name = "ShaderNameTextBox";
            this.ShaderNameTextBox.Size = new System.Drawing.Size(202, 23);
            this.ShaderNameTextBox.TabIndex = 4;
            // 
            // ShaderNameLabel
            // 
            this.ShaderNameLabel.AutoSize = true;
            this.ShaderNameLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.ShaderNameLabel.Location = new System.Drawing.Point(3, 6);
            this.ShaderNameLabel.Name = "ShaderNameLabel";
            this.ShaderNameLabel.Size = new System.Drawing.Size(49, 17);
            this.ShaderNameLabel.TabIndex = 5;
            this.ShaderNameLabel.Text = "Name:";
            // 
            // NodeSelector
            // 
            this.NodeSelector.Location = new System.Drawing.Point(6, 19);
            this.NodeSelector.Name = "NodeSelector";
            this.NodeSelector.Size = new System.Drawing.Size(212, 730);
            this.NodeSelector.TabIndex = 0;
            // 
            // ShaderEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.ShaderNameLabel);
            this.Controls.Add(this.ShaderNameTextBox);
            this.Controls.Add(this.NodeSelectorBox);
            this.Controls.Add(this.NodeProperties);
            this.Controls.Add(this.NodesPanel);
            this.Name = "ShaderEditor";
            this.Size = new System.Drawing.Size(1406, 761);
            this.NodeSelectorBox.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Panel NodesPanel;
        private System.Windows.Forms.PropertyGrid NodeProperties;
        private System.Windows.Forms.GroupBox NodeSelectorBox;
        private System.Windows.Forms.TextBox ShaderNameTextBox;
        private System.Windows.Forms.Label ShaderNameLabel;
        private System.Windows.Forms.TreeView NodeSelector;
    }
}
