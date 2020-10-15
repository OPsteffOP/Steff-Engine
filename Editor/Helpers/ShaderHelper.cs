using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Helpers
{
    class ShaderHelper
    {
        public static List<string> GetShaderNames()
        {
            List<string> shaderNames = new List<string>();

            string shaderFolderPath = @"C:\Users\Administrator\Perforce\STEFF-DESKTOP\Personal\Engine\Dev\Resources\Configs";
            foreach (string shaderFilePath in Directory.GetFiles(shaderFolderPath, "*.sSE", SearchOption.TopDirectoryOnly))
            {
                string shaderFileName = Path.GetFileNameWithoutExtension(shaderFilePath);
                shaderNames.Add(shaderFileName);
            }

            return shaderNames;
        }
    }
}
