using Editor.Shaders;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing.Design;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using System.Windows.Forms.Design;

namespace Editor.Helpers
{
    // https://stackoverflow.com/questions/3491556/how-to-display-a-dynamic-object-in-property-grid

    public class CustomProperty
    {
        public CustomProperty() { }

        public CustomProperty(string category, string name, string placeholderName, string description, Type type, bool readOnly, bool visible)
        {
            Category = category;
            Name = name;
            PlaceholderName = placeholderName;
            Description = description;
            TypeString = type.ToString();
            ReadOnly = readOnly;
            Visible = visible;
        }

        public string Category { get; set; }
        public string Name { get; set; }
        public string PlaceholderName { get; set; }
        public string Description { get; set; }

        public string TypeString { get; set; }
        [JsonIgnore]
        public Type Type { get { return Type.GetType(TypeString); } }
        public object DefaultValue { get; set; }
        [JsonIgnore]
        public object Value { get; set; }

        public bool ReadOnly { get; set; }
        public bool Visible { get; set; }
    }

    [TypeConverter(typeof(CustomObjectType.CustomObjectConverter))]
    public class CustomObjectType
    {
        [Category("\tNode")] // \t to make sure the category is on top (no visual effect)
        [ReadOnly(true)]
        [Description("The name of the selected node")]
        public string Name { get; set; }

        [Category("\tNode")] // \t to make sure the category is on top (no visual effect)
        [ReadOnly(true)]
        [Description("The unique name of the selected node")]
        public string UniqueName { get; set; }

        private List<CustomProperty> props = new List<CustomProperty>();
        [Browsable(false)]
        public List<CustomProperty> Properties { get { return props; } set { props = value; } }

        private Dictionary<string, object> values = new Dictionary<string, object>();

        public object this[string name]
        {
            get { object val; values.TryGetValue(name, out val); return val; }
            set { values.Remove(name); }
        }

        private class CustomObjectConverter : ExpandableObjectConverter
        {
            public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes)
            {
                var stdProps = base.GetProperties(context, value, attributes);
                CustomObjectType obj = value as CustomObjectType;
                List<CustomProperty> customProps = obj == null ? null : obj.Properties;
                PropertyDescriptor[] props = new PropertyDescriptor[stdProps.Count + (customProps == null ? 0 : customProps.Count)];
                stdProps.CopyTo(props, 0);
                if (customProps != null)
                {
                    int index = stdProps.Count;
                    foreach (CustomProperty prop in customProps)
                    {
                        props[index++] = new CustomPropertyDescriptor(prop);
                    }
                }
                return new PropertyDescriptorCollection(props);
            }
        }

        private class CustomPropertyDescriptor : PropertyDescriptor
        {
            private readonly CustomProperty prop;
            public CustomPropertyDescriptor(CustomProperty prop) : base(prop.Name, null)
            {
                this.prop = prop;
            }
            public override string Category { get { return prop.Category; } }
            public override string Description { get { return prop.Description; } }
            public override string Name { get { return prop.Name; } }
            public override bool ShouldSerializeValue(object component) { return prop.Value != null; }
            public override void ResetValue(object component) { prop.Value = null; }
            public override bool IsReadOnly { get { return prop.ReadOnly; } }
            public override Type PropertyType { get { return prop.Type; } }
            public override bool CanResetValue(object component) { return true; }
            public override Type ComponentType { get { return typeof(CustomObjectType); } }
            public override void SetValue(object component, object value) { prop.Value = value; }
            public override object GetValue(object component) { return prop.Value ?? prop.DefaultValue; }
        }
    }
}