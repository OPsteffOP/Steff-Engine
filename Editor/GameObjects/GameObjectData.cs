using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.GameObjects
{
    struct GameObjectData
    {
        public GameObjectData(String name, uint id)
        {
            this.name = name;
            this.id = id;
        }

        public String name;
        public uint id;

        public override String ToString()
        {
            return name;
        }
    }
}
