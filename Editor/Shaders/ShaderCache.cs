using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Shaders
{
    class ShaderCache
    {
        public static uint GetShaderIDByName(string shaderName)
        {
            if (shaderCache.ContainsValue(shaderName))
            {
                foreach (KeyValuePair<uint, string> pair in shaderCache)
                {
                    if (pair.Value == shaderName)
                        return pair.Key;
                }

                return 0;
            }
            else
            {
                // Shader not cached, load it
                uint shaderID = Bridge.LoadShader(shaderName);
                shaderCache.Add(shaderID, shaderName);
                return shaderID;
            }
        }

        public static string GetShaderNameByID(uint shaderID)
        {
            if (shaderCache.ContainsKey(shaderID))
                return shaderCache[shaderID];

            return null;
        }

        private static Dictionary<uint, string> shaderCache = new Dictionary<uint, string>() { { 0, "DefaultShader" } };
    }
}
