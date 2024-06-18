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

// todo
// infinite health

struct Vector3
{
	float x = 1.f;
	float y = 1.f;
	float z = 1.f;
};

int main()
{
	// Display local player information

	Memory memory{ "ac_client.exe" };

	const auto baseAddress = memory.GetModuleAddress("ac_client.exe");
	const auto localPlayerEntity = memory.Read<uintptr_t>(baseAddress + offset::localPlayerEntity);

	int localPlayerHealth = memory.Read<int>(localPlayerEntity + offset::iHealth);
	int localPlayerArmor = memory.Read<int>(localPlayerEntity + offset::iArmor);
	int localPlayerLoadedAmmo = memory.Read<int>(localPlayerEntity, { offset::currentWeaponObject, offset::ammoLoaded, 0x0 });
	int localPlayerReserveAmmo = memory.Read<int>(localPlayerEntity, { offset::currentWeaponObject, offset::ammoReserve, 0x0 });
	int localPlayerGranade = memory.Read<int>(localPlayerEntity, { offset::granadeObject,  offset::ammoLoaded, 0x0 });
	int weaponId = memory.Read<int>(localPlayerEntity, { offset::currentWeaponObject, offset::iWeaponId });
	bool isDead = memory.Read<bool>(localPlayerEntity + offset::bIsDead);
	Vector3 playerPosition = memory.Read<Vector3>(localPlayerEntity + offset::playerPos);

	char playerName[16] = { 0 };
	memory.ReadChar<char>(localPlayerEntity + offset::cPlayerName, playerName, 16);

	while (true)
	{
		isDead = memory.Read<bool>(localPlayerEntity + offset::bIsDead);
		memory.ReadChar<char>(localPlayerEntity + offset::cPlayerName, playerName, 16);

		if (!isDead)
		{
			playerPosition = memory.Read<Vector3>(localPlayerEntity + offset::playerPos);
			localPlayerHealth = memory.Read<int>(localPlayerEntity + offset::iHealth);
			localPlayerArmor = memory.Read<int>(localPlayerEntity + offset::iArmor);
			localPlayerGranade = memory.Read<int>(localPlayerEntity, { offset::granadeObject, offset::ammoLoaded, 0x0 });
			weaponId = memory.Read<int>(localPlayerEntity, { offset::currentWeaponObject, offset::iWeaponId });

			if (weaponId != weaponId::knife && weaponId != weaponId::granade)
			{
				localPlayerLoadedAmmo = memory.Read<int>(localPlayerEntity, { offset::currentWeaponObject, offset::ammoLoaded, 0x0 });
				localPlayerReserveAmmo = memory.Read<int>(localPlayerEntity, { offset::currentWeaponObject, offset::ammoReserve, 0x0 });
			}
		}

		std::cout << "Local player name: " << playerName << std::endl;
		std::cout << "Local player health: " << localPlayerHealth << std::endl;
		std::cout << "Local player armor: " << localPlayerArmor << std::endl;
		std::cout << "Local player loaded ammo: " << localPlayerLoadedAmmo << std::endl;
		std::cout << "Local player reserve ammo: " << localPlayerReserveAmmo << std::endl;
		std::cout << "Local player granade: " << localPlayerGranade << std::endl;
		std::cout << "Local player position: (" << playerPosition.x << ", " << playerPosition.y << ", " << playerPosition.z << ")" << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		system("cls");
	}

	return 0;
}