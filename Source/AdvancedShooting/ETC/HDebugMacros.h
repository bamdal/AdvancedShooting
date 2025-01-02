#pragma once

#include "DrawDebugHelpers.h"

#define HDRAW_SPHERE(Location) \
if(GetWorld()) \
	DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, true);

#define HDRAW_SPHERE_COLOR(Location, Color) \
if(GetWorld()) \
	DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);

#define HDRAW_SPHERE_SingleFrame(Location) \
if (GetWorld()) \
	DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);

#define HDRAW_SPHERE_Custom_SingleFrame(Location, Radius, Color) \
if (GetWorld()) \
	DrawDebugSphere(GetWorld(), Location, Radius, 12, Color, false, -1.f);

#define HDRAW_LINE(StartLocation, EndLocation) \
if(GetWorld()) \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);

#define HDRAW_LINE_SingleFrame(StartLocation, EndLocation) \
if (GetWorld()) \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);

#define HDRAW_POINT(Location) \
if(GetWorld()) \
	DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);

#define HDRAW_POINT_SingleFrame(Location) \
if (GetWorld()) \
	DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, -1.f);

#define HDRAW_VECTOR(StartLocation, EndLocation) \
if(GetWorld()) \
{ \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f); \
	DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, true); \
}

#define HDRAW_VECTOR_SingleFrame(StartLocation, EndLocation) \
if (GetWorld()) \
{ \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f); \
	DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, false, -1.f); \
}

//-------------------------------------------------------------------------------------------------

DECLARE_LOG_CATEGORY_EXTERN(Headcooler, Log, All);

// 함수 이름 + 코드 라인 출력
#define HLOG_CALLINFO ( FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")") )

#define HLOG_S( Verbosity ) \
	UE_LOG( Headcooler, Verbosity, TEXT("%s"), *HLOG_CALLINFO )

#define HLOG( Verbosity, Format, ... ) \
	UE_LOG( Headcooler, Verbosity, TEXT("%s %s"), *HLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__) )

#define HCHECK( Expr, ... ) \
{ \
	if( !(Expr) ) \
	{ \
		HLOG( Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'") ); \
        return __VA_ARGS__; \
	} \
}

//-----------------------------------------------------------------------------------
// AActor 하위 클래스에서만 사용

#define HLOG_NETMODE \
( \
	(GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT %d"), UE::GetPlayInEditorID()) : \
	( \
		(GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : \
		( \
			(GetNetMode() == ENetMode::NM_ListenServer) ? TEXT("LISTEN_SERVER") : TEXT("DEDICATED_SERVER") \
		) \
	) \
)

#define HLOG_LOCALROLE  *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))
#define HLOG_REMOTEROLE  *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))

#define HLOG_NET_LOG( Verbosity, Format, ... ) \
	UE_LOG(Headcooler, Verbosity, TEXT("[%s][%s/%s] %s %s"), \
	HLOG_NETMODE, HLOG_LOCALROLE, HLOG_REMOTEROLE, *HLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__) )