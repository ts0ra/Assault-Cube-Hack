#include "backend/memory.h"
#include "backend/offset.h"

#include <iostream>
#include <thread>

// Writing char
// const char newName[16] = "anjing naga";
// memory.Write(localPlayerEntity + offset::cPlayerName, newName);

// Reading char
// char localPlayerName[MAX_PLAYER_NAME];
// memory.ReadChar<char>(localPlayerEntity + offset::cPlayerName, localPlayerName, MAX_PLAYER_NAME);

int main()
{
	// Display local player information

	Memory memory{ "ac_client.exe" };

	const auto baseAddress = memory.GetModuleAddress("ac_client.exe");
	const auto localPlayerEntity = memory.Read<uintptr_t>(baseAddress + offset::localPlayerEntity);
	const auto currentWeaponObject = memory.Read<uintptr_t>(localPlayerEntity + offset::currentWeaponObject);
	const auto ammoReservePointer = memory.Read<uintptr_t>(currentWeaponObject + offset::ammoReserve);

	// int ammoReserve = memory.Read<int>(ammoReservePointer + 0);
	const auto localPlayerHealth = memory.Read<int>(localPlayerEntity + offset::iHealth);
	const auto localPlayerArmor = memory.Read<int>(localPlayerEntity + offset::iArmor);

	int ammoReserve = memory.Read<int>(localPlayerEntity, { offset::currentWeaponObject, offset::ammoLoaded });

	while (true)
	{
		std::cout << "Current ammo: " << ammoReserve << std::endl;
		ammoReserve = ammoReserve = memory.Read<int>(localPlayerEntity, { offset::currentWeaponObject, offset::ammoLoaded });
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		system("cls");
	}

	return 0;
}