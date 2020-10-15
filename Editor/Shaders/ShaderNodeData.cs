using Editor.Helpers;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Drawing;
using System.Dynamic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Editor.Shaders
{
    // https://docs.microsoft.com/en-us/dotnet/standard/serialization/system-text-json-how-to

    public enum PinType
    {
        IN,
        OUT
    }

    public enum ShaderVariableType
    {
        INT,
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        STRING,
        TEXTURE
    }

    public enum VariableScope
    {
        GLOBAL,
        SHADER
    }

    [Serializable]
    public class ShaderPinData : ICloneable
    {
        public ShaderPinData() { }

        public ShaderPinData(String name, PinType type, ShaderVariableType valueType, ShaderType shaderType)
        {
            Name = name;
            Type = type;
            ValueType = valueType;
            ShaderTypes.Add(shaderType);
        }

        public string Name { get; set; }
        public PinType Type { get; set; }
        public ShaderVariableType ValueType { get; set; }

        public string GlobalVariable { get; set; }
        public List<string> ShaderCode { get; set; }

        [JsonIgnore]
        public RadioButton ConnectedPin { get; set; } // the radiobutton this pin is connected to

        [JsonIgnore]
        public Point MousePoint { get; set; } // gets set when the user is dragging the pin but not yet connected to a radiobutton

        [JsonIgnore]
        public List<ShaderType> ShaderTypes { get; set; } = new List<ShaderType>(); // only valid for the pins in the output node - specifies in which shader file the output should be put in

        public object Clone()
        {
            return CloneHelper.Copy(this);
        }

        public override string ToString()
        {
            return Name;
        }

        public string GetVariableName(VariableScope type)
        {
            return "variable" + GetVariableID(type);
        }

        public int GetVariableID(VariableScope type)
        {
            if (!variableIDs.ContainsKey(type))
                variableIDs.Add(type, nextVariableID++);

            return variableIDs[type];
        }

        public void SetVariableID(VariableScope type, int id)
        {
            variableIDs[type] = id;
        }

        private static int nextVariableID = 0;
        private Dictionary<VariableScope, int> variableIDs = new Dictionary<VariableScope, int>();
    }

    [Serializable]
    public class ShaderNodeData : ICloneable
    {
        public ShaderNodeData() { }

        public ShaderNodeData(string title, List<ShaderPinData> pinData)
        {
            Title = title;
            PinData = pinData;
        }

        public string Title { get; set; }
        public string UniqueTitle { get; set; }

        public List<ShaderPinData> PinData { get; set; }
        public List<CustomProperty> Properties { get; set; }

        [JsonIgnore]
        public bool IsProcessed { get; set; } = false;

        public object Clone()
        {
            return CloneHelper.Copy(this);
        }

        public override string ToString()
        {
            return UniqueTitle;
        }
    }
}
