// Fill out your copyright notice in the Description page of Project Settings.


#include "TDGameModeBase.h"
#include "TDPlayerController.h"

ATDGameModeBase::ATDGameModeBase()
{
	PlayerControllerClass = ATDPlayerController::StaticClass();
	DefaultPawnClass = nullptr;
}