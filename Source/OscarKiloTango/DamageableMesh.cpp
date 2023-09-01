// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageableMesh.h"

// Sets default values
ADamageableMesh::ADamageableMesh()
{

}

void ADamageableMesh::ApplyDamage() 
{
	Destroy();
}
