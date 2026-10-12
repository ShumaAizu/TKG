xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 18;
 0.00000;5.00000;0.00000;,
 3.00000;2.50000;-2.16507;,
 0.00000;2.50000;-4.33013;,
 3.00000;2.50000;2.16507;,
 0.00000;2.50000;4.18013;,
 -3.00000;2.50000;2.16507;,
 -2.85000;2.50000;-2.16507;,
 0.00000;2.50000;-4.33013;,
 3.00000;2.50000;-2.16507;,
 3.00000;-2.50000;-2.16507;,
 0.00000;-2.50000;-4.13013;,
 2.85000;-2.50000;2.16507;,
 0.00000;-2.50000;4.33013;,
 -3.00000;-2.50000;2.16507;,
 -3.00000;-2.50000;-2.16507;,
 0.00000;-2.50000;-4.13013;,
 0.00000;-5.00000;-0.00000;,
 0.00000;-5.00000;-0.00000;;
 
 18;
 3;0,1,2;,
 3;0,3,1;,
 3;0,4,3;,
 3;0,5,4;,
 3;0,6,5;,
 3;0,2,6;,
 4;7,8,9,10;,
 4;8,3,11,9;,
 4;3,4,12,11;,
 4;4,5,13,12;,
 4;5,6,14,13;,
 4;6,2,15,14;,
 3;10,9,16;,
 3;9,11,16;,
 3;11,12,17;,
 3;12,13,17;,
 3;13,14,17;,
 3;14,15,17;;
 
 MeshMaterialList {
  1;
  18;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;0.850000;;
   75.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\source\\DirectX\\3DActionGame_000\\DirectX_003\\data\\TEXTURE\\ice000.jpg";
   }
  }
 }
 MeshNormals {
  21;
  -0.005242;0.999984;0.002148;,
  -0.008350;0.865995;-0.499983;,
  0.510226;0.824416;-0.244964;,
  0.504728;0.825217;0.253507;,
  0.000000;0.858223;0.513276;,
  -0.767904;0.616712;0.173172;,
  -0.781732;0.477572;-0.401024;,
  0.226550;-0.654302;-0.721501;,
  0.778345;-0.491256;-0.390957;,
  0.782138;-0.478193;0.399489;,
  0.242777;-0.658877;0.711997;,
  -0.896491;-0.300433;0.325642;,
  -0.777175;-0.481112;-0.405623;,
  0.005245;-0.999977;-0.004304;,
  0.566637;-0.016215;-0.823808;,
  0.888932;-0.017708;-0.457697;,
  0.885217;-0.006386;0.465135;,
  0.581490;0.003581;0.813546;,
  -0.571430;0.012162;0.820561;,
  -0.576755;-0.007707;-0.816881;,
  -0.339422;-0.814613;0.470317;;
  18;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,1,6;,
  4;14,15,8,7;,
  4;15,16,9,8;,
  4;16,17,10,9;,
  4;18,18,11,18;,
  4;5,6,12,11;,
  4;6,19,19,12;,
  3;7,8,13;,
  3;8,9,13;,
  3;9,10,13;,
  3;10,20,13;,
  3;11,12,13;,
  3;12,7,13;;
 }
 MeshTextureCoords {
  18;
  0.750000;0.250000;,
  1.150507;0.375000;,
  1.000000;0.375000;,
  0.349493;0.375000;,
  0.500000;0.375000;,
  0.650507;0.375000;,
  0.853397;0.375000;,
  0.000000;0.375000;,
  0.150507;0.375000;,
  0.150507;0.625000;,
  0.000000;0.625000;,
  0.353397;0.625000;,
  0.500000;0.625000;,
  0.650507;0.625000;,
  0.849493;0.625000;,
  1.000000;0.625000;,
  -0.250000;0.750000;,
  0.750000;0.750000;;
 }
}
