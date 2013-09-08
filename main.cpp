#include <iostream>

#include "Engine.h"
#include "Render.h"
#include "Resource.h"

#include "GL_31.h"

#include "Collada.h"

using namespace std;
using namespace TE;




void main(int argc, char* argv[])
{
	Engine.Resource->Setup_FileSystem(argv[0]);

	Collada_Loader C;
	C.Load("Box.dae");
	//Col_Library_Geometries Lib = Collada::Load_LibGeometries("Box.dae");

	Engine.MainLoop_Start();
	getchar();

}