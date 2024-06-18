#pragma once

namespace offset
{
	constexpr auto playerEntityList = 0x18AC04; // uintptr_t, player entity list

	constexpr auto localPlayerEntity = 0x18AC00; // uintptr_t, local player entity
	constexpr auto cPlayerName = 0x205; // char[16], local player name
	constexpr auto playerHead = 0x4; // vec3, local player head
	constexpr auto playerPos = 0x28; // vec3, local player position
	constexpr auto viewAngles = 0x34; // vec2, could be vec3 but roll is always 0
	constexpr auto iHealth = 0xEC;
	constexpr auto iArmor = 0xF0;
	constexpr auto iTeamNum = 0x30C;
	constexpr auto bIsDead = 0x318;

	constexpr auto currentWeaponObject = 0x364; // uintptr_t, current weapon object (localPlayerEntity + currentWeaponObject)
	constexpr auto iWeaponId = 0x4; // int, weapon id
	constexpr auto cWeaponName = 0x17; // char[16]
	constexpr auto ammoReserve = 0x10; // uintptr_t, ammo reserve
	constexpr auto ammoLoaded = 0x14; // uintptr_t, ammo loaded

	constexpr auto gunInfo = 0xC; // uintptr_t, gun info (currentWeaponObject + gunInfo)
	constexpr auto cGunName = 0x0; // char[?]
	constexpr auto sWeaponShootFX = 0x42;
	constexpr auto sWeaponReloadFX = 0x44;
	constexpr auto sReloadTime = 0x46;
	constexpr auto sFireRate = 0x48;
	constexpr auto sDamage = 0x4A;
	constexpr auto sRecoilKnockback = 0x54;
	constexpr auto sMagSize = 0x56;
	constexpr auto sRecoilPunch = 0x60;
	constexpr auto sToggleFullAuto = 0x66;


	// find offset
	// player team side
	// view matrix
	// current weapon
}

enum weaponName
{
	knife = 0,
	pistol = 1,
	ar = 2,
};