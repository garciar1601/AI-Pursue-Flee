using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace OBJ_Converter
{
    class Program
    {
        static List<Vec3> positions = new List<Vec3>();
        static List<Vec2> uvs = new List<Vec2>();
        static List<Vec3> normals = new List<Vec3>();
        static List<Vertex> vertices = new List<Vertex>();

        static List<ushort> indices = new List<ushort>();
        static void Main()
        {
            ReadData("Pyramid.obj");
            calcTangents();
            WriteData("Pyramid.bin");
        }

        private static void WriteData(string fileName)
        {
            BinaryWriter bw = new BinaryWriter(File.Create(fileName));

            bw.Write((uint)vertices.Count);
            bw.Write((uint)indices.Count);
            float o = 1;
            foreach (Vertex v in vertices)
            {
                bw.Write(v.position.X);
                bw.Write(v.position.Y);
                bw.Write(v.position.Z);

                bw.Write(v.tangent.X);
                bw.Write(v.tangent.Y);
                bw.Write(v.tangent.Z);
                bw.Write(o);

                Console.WriteLine(v.tangent.X);
                Console.WriteLine(v.tangent.Y);
                Console.WriteLine(v.tangent.Z);

                bw.Write(v.normal.X);
                bw.Write(v.normal.Y);
                bw.Write(v.normal.Z);

                bw.Write(v.uv.X);
                bw.Write(v.uv.Y);
            }

            foreach (ushort s in indices)
            {
                bw.Write(s);
            }
            bw.Close();
        }

        static Random randy = new Random();
        private static float RandomFloat()
        {
            return (float)randy.NextDouble();
        }


        private static void ReadData(string fileName)
        {
            StreamReader sr = new StreamReader(fileName);
            string line;
            while ((line = sr.ReadLine()) != null)
            {
                string[] lineParts = line.Split();
                Console.WriteLine(lineParts[0]);
                switch (lineParts[0])
                {
                    case "v":
                        Vec3 v = new Vec3
                        {
                            X = float.Parse(lineParts[1]),
                            Y = float.Parse(lineParts[2]),
                            Z = float.Parse(lineParts[3])
                        };
                        Console.Write(lineParts[1] + " ");
                        Console.Write(lineParts[2] + " ");
                        Console.WriteLine(lineParts[3]);
                        positions.Add(v);
                        break;
                    case "vt":
                        Vec2 vt = new Vec2
                        {
                            X = float.Parse(lineParts[1]),
                            Y = float.Parse(lineParts[2])
                        };
                        Console.Write(lineParts[1] + " ");
                        Console.WriteLine(lineParts[2]);
                        uvs.Add(vt);
                        break;
                    case "vn":
                        Vec3 vn = new Vec3
                        {
                            X = float.Parse(lineParts[1]),
                            Y = float.Parse(lineParts[2]),
                            Z = float.Parse(lineParts[3])
                        };
                        Console.Write(lineParts[1] + " ");
                        Console.Write(lineParts[2] + " ");
                        Console.WriteLine(lineParts[3]);
                        normals.Add(vn);
                        break;
                    case "f":
                        AddFace(lineParts);
                        break;
                }
            }
        }

        static void AddFace(string[] lineParts)
        {
            AddAVert(lineParts[1].Split('/'));
            AddAVert(lineParts[2].Split('/'));
            AddAVert(lineParts[3].Split('/'));
            Console.Write(lineParts[1] + " ");
            Console.Write(lineParts[2] + " ");
            Console.WriteLine(lineParts[3]);
        }

        private static void AddAVert(string[] vertParts)
        {
            string[] vertIndices = vertParts;
            Vertex meVert = makeAVert(vertIndices);
            int index = vertices.IndexOf(meVert);
            if (index == -1)
            {
                vertices.Add(meVert);
                index = vertices.Count - 1;
            }
            indices.Add((ushort)index);
        }

        private static Vertex makeAVert(string[] vertIndices)
        {
            Vertex meVert;
            meVert.position = positions[int.Parse(vertIndices[0]) - 1];
            meVert.uv = uvs[int.Parse(vertIndices[1]) - 1];
            meVert.normal = normals[int.Parse(vertIndices[2]) - 1];
            meVert.tangent.X = 0;
            meVert.tangent.Y = 0;
            meVert.tangent.Z = 0;
            return meVert;
        }

        private static Vec3 add(Vec3 vec1, Vec3 vec2)
        {
            Vec3 ret;
            ret.X = vec1.X + vec2.X;
            ret.Y = vec1.Y + vec2.Y;
            ret.Z = vec1.Z + vec2.Z;
            return ret;
        }

        private static Vec3 subtract(Vec3 vec1, Vec3 vec2)
        {
            Vec3 ret;
            ret.X = vec1.X - vec2.X;
            ret.Y = vec1.Y - vec2.Y;
            ret.Z = vec1.Z - vec2.Z;
            return ret;
        }

        private static Vec3 multiply(float scale, Vec3 vec)
        {
            Vec3 ret;
            ret.X = scale * vec.X;
            ret.Y = scale * vec.Y;
            ret.Z = scale * vec.Z;
            return ret;
        }

        private static float dot(Vec3 vec1, Vec3 vec2)
        {
            float ret = (vec1.X * vec2.X) + (vec1.Y * vec2.Y) + (vec1.Z + vec2.Z);
            return ret;
        }

        private static Vec3 normalize(Vec3 vec)
        {
            Vec3 ret;
            float size = length(vec);
            ret.X = vec.X / size;
            ret.Y = vec.Y / size;
            ret.Z = vec.Z / size;
            return ret;
        }

        private static float length(Vec3 vec)
        {
            float ret = (float)Math.Sqrt((vec.X * vec.X) + (vec.Y * vec.Y) + (vec.Z * vec.Z));
            return ret;
        }

        private static void calcTangents()
        {
            for (int i = 0 ; i < indices.Count ; i += 3) {
                Vertex v0 = vertices[indices[i]];
                Vertex v1 = vertices[indices[i+1]];
                Vertex v2 = vertices[indices[i+2]];

                Vec3 Edge1 = subtract(v1.position, v0.position);
                Vec3 Edge2 = subtract(v2.position, v0.position);

                float DeltaU1 = v1.uv.X - v0.uv.X;
                float DeltaV1 = v1.uv.Y - v0.uv.Y;
                float DeltaU2 = v2.uv.Y - v0.uv.Y;
                float DeltaV2 = v2.uv.Y - v0.uv.Y;

                float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

                Vec3 Tangent;

                Tangent.X = f * (DeltaV2 * Edge1.X - DeltaV1 * Edge2.X);
                Tangent.Y = f * (DeltaV2 * Edge1.Y - DeltaV1 * Edge2.Y);
                Tangent.Z = f * (DeltaV2 * Edge1.Z - DeltaV1 * Edge2.Z);

                v0.tangent = add(v0.tangent, Tangent);
                v1.tangent = add(v1.tangent, Tangent);
                v2.tangent = add(v2.tangent, Tangent);
                vertices[indices[i]] = v0;
                vertices[indices[i+1]] = v1;
                vertices[indices[i+2]] = v2;
            }

            for (int i = 0 ; i < vertices.Count ; i++) {
                Vertex newVert;
                newVert.position = vertices[i].position;
                newVert.uv = vertices[i].uv;
                newVert.normal = vertices[i].normal;
                newVert.tangent = normalize(subtract(vertices[i].tangent, multiply(dot(vertices[i].normal, vertices[i].tangent), vertices[i].normal)));
                vertices[i] = newVert;
            } 
        }

    }
}

struct Vec3
{
    public float X;
    public float Y;
    public float Z;
}

struct Vec2
{
    public float X;
    public float Y;
}

struct Vertex
{
   public Vec3 position;
   public Vec2 uv;
   public Vec3 normal;
   public Vec3 tangent;
   
}