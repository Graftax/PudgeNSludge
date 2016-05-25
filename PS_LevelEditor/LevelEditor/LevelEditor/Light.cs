using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace LevelEditor
{
    class Light
    {
        int vecPos = -1;
        int type = -1;

        Color diffuse;
        Color ambient;
        Color specular;

        float specpower = 0.0f;
        float specIntensity = 0.0f;

        public int VecPos
        {
            get { return vecPos; }
            set { vecPos = value; }
        }

        public int Type
        {
            get { return type; }
            set { type = value; }
        }

        public Color Diffuse
        {
            get { return diffuse; }
            set { diffuse = value; }
        }

        public Color Ambient
        {
            get { return ambient; }
            set { ambient = value; }
        }

        public Color Specular
        {
            get { return specular; }
            set { specular = value; }
        }

        public float Specpower
        {
            get { return specpower; }
            set { specpower = value; }
        }

        public float SpecIntensity
        {
            get { return specIntensity; }
            set { specIntensity = value; }
        }

        public override string ToString()
        {
            string name = "Light";

            return name;
        }
    }
}
