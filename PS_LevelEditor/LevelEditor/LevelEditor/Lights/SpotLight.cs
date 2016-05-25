using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class SpotLight : Light
    {
        VEC3F position;
        VEC3F direction;
        VEC3F atten;
        float expo;
        float cutoff;
        float radius;

        public VEC3F Position
        {
            get { return position; }
            set { position = value; }
        }

        public VEC3F Direction
        {
            get { return direction; }
            set { direction = value; }
        }

        public VEC3F Atten
        {
            get { return atten; }
            set { atten = value; }
        }

        public float Expo
        {
            get { return expo; }
            set { expo = value; }
        }

        public float Cutoff
        {
            get { return cutoff; }
            set { cutoff = value; }
        }

        public float Radius
        {
            get { return radius; }
            set { radius = value; }
        }

        public override string ToString()
        {
            string name = "Spot Light";

            return name;
        }
    }
}
