#pragma once

#include "DrawDebugHelpers.h"

// BeginPlay() => bpresistentline=true
#define DRAW_SPHERE(StartLocation) if(GetWorld()) DrawDebugSphere(GetWorld(), StartLocation, 25.f, 24, FColor::Red, true, -1.f, 1.f);

#define DRAW_SPHERE_Color(StartLocation,Color) if (GetWorld()) DrawDebugSphere(GetWorld(),StartLocation,5.f,24,Color,false,5.f);

#define Draw_LINE(StartLocation,EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(),StartLocation, EndLocation, FColor::Blue, true, -1.f, 0, 1.f);

#define DRAW_POINT_START(StartLocation) if(GetWorld()) DrawDebugPoint(GetWorld(),StartLocation,20.f,FColor::Green,true);

#define DRAW_POINT_END(EndLocation) if(GetWorld()) DrawDebugPoint(GetWorld(),EndLocation,20.f,FColor::Cyan,true);

#define DRAW_CAPSULE(StartLocation) if(GetWorld()) DrawDebugCapsule(GetWorld(),StartLocation,10.f,10.f,FQuat::Identity,FColor::Cyan,true);

#define DRAW_VECTOR(StartLocation,EndLocation) do {\
	if(GetWorld()) {\
		Draw_LINE(StartLocation,EndLocation);\
		DRAW_POINT_START(StartLocation);\
		DRAW_POINT_END(EndLocation);\
	}\
}while(0)

// TickEvent() => bpresistentline=false, lifetime by default -1.f
#define DRAW_SPHERE_SingleFrame(StartLocation) if (GetWorld()) DrawDebugSphere(GetWorld(),StartLocation,25,24,FColor::Red,false,-1.f);

#define DRAW_LINE_SingleFrame(StartLocation,EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Blue,false,-1.f);

#define DRAW_POINT_START_SingleFrame(StartLocation) if(GetWorld()) DrawDebugPoint(GetWorld(),StartLocation,20.f,FColor::Green,false,-1.f);

#define DRAW_POINT_END_SingleFrame(EndLocation) if(GetWorld()) DrawDebugPoint(GetWorld(),EndLocation,20.f,FColor::Cyan,false,-1.f);

#define DRAW_VECTOR_SingleFrame(StartLocation,EndLocation) do { \
	if(GetWorld()) { \
		DRAW_LINE_SingleFrame(StartLocation, EndLocation); \
		DRAW_POINT_START_SingleFrame(StartLocation); \
		DRAW_POINT_END_SingleFrame(EndLocation); \
	} \
} while(0)