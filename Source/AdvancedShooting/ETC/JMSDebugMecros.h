#pragma once

#include "DrawDebugHelpers.h"

#define JMSDRAW_SPHERE(Location) \
if(GetWorld()) \
	DrawDebugSphere(GetWorld(), Location, 25.0f, 12, FColor::Red, true);

#define JMSDRAW_SPHERE_COLOR(Location,Color) \
if(GetWorld()) \
	DrawDebugSphere(GetWorld(), Location, 50.0f, 12, Color, true);

// Tick에서 사용
#define JMSDRAW_SPHERE_SingleFrame(Location) \
if(GetWorld()) \
	DrawDebugSphere(GetWorld(), Location, 25.0f, 12, FColor::Red, false,-1.0f);


#define JMSDRAW_LINE(StartLocation, EndLocation)\
if(GetWorld()) \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, true, -1.0f,0,1.0f);

// Tick에서 사용
#define JMSDRAW_LINE_SingleFrame(StartLocation, EndLocation)\
if(GetWorld()) \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, -1.0f,0,1.0f);

#define JMSDRAW_POINT(Location) \
if(GetWorld()) \
	DrawDebugPoint(GetWorld(), Location, 15, FColor::Red,true);

// Tick에서 사용
#define JMSDRAW_POINT_SingleFrame(Location) \
if(GetWorld()) \
	DrawDebugPoint(GetWorld(), Location, 15, FColor::Red,false,-1.0f);


#define JMSDRAW_VECTOR(StartLocation, EndLocation) \
if (GetWorld()) \
{ \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, true, 100); \
	DrawDebugPoint(GetWorld(), EndLocation, 15, FColor::Red, true); \
}

// Tick에서 사용
#define JMSDRAW_VECTOR_SingleFrame(StartLocation, EndLocation) \
if (GetWorld()) \
{ \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, -1.0f,0,1.0f); \
	DrawDebugPoint(GetWorld(), EndLocation, 15, FColor::Red,false,-1.0f); \
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

// AActor의 하위클래스에서만 사용 ---------------------------------------------------------------------------------

#define JMSLOG_NETMODE \
( \
    (GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT %d"), UE::GetPlayInEditorID()) : \
    ( \
        (GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : \
        ( \
            (GetNetMode() == ENetMode::NM_ListenServer) ? TEXT("LISTEN_SERVER") : TEXT("DEDICATED_SERVER") \
        ) \
    ) \
)

#define JMSLOG_LOCALROLE *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))
#define JMSLOG_REMOTEROLE *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))

#define ROLE_TO_STRING(Value) FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByIndex(static_cast<int32>(Value))

#define JMSLOG_NET_LOG( Verbosity, Format, ... ) \
    UE_LOG(Headcooler, Verbosity, TEXT("[%s][%s/%s] %s %s"), \
    JMSLOG_NETMODE, JMSLOG_LOCALROLE, JMSLOG_REMOTEROLE, *HLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__) )