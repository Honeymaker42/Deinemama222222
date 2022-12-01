#include "memory.h"
#include <cstddef>
#include "thread"
#include "../Header/memory.h"
#include <iostream>
namespace offset
{
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDE7964;
	constexpr ::std::ptrdiff_t dwEntityList = 0x4DFCE74;
	constexpr ::std::ptrdiff_t dwForceAttack = 0x322AC7C;


	constexpr ::std::ptrdiff_t m_iHealth = 0x100;
	constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
	constexpr ::std::ptrdiff_t m_iCrosshairId = 0x11838;
}


int main()
{

	const auto memory = Memory{ "csgo.exe" };
	const auto client = memory.GetModuleAddress("client.dll");

	std::cout << std::hex << "client.dll -> 0x" << client << std::dec << std::endl;


	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (!GetAsyncKeyState(0x43))
			continue;

		const auto& localPlayer = memory.Read<std::uintptr_t>(client + offset::dwLocalPlayer);
		const auto& localHelth = memory.Read<std::int32_t>(localPlayer + offset::m_iHealth);


		if (!localHelth)
			continue;


		const auto& crosshairid = memory.Read<std::int32_t>(localPlayer + offset::m_iCrosshairId);


		if (!crosshairid || crosshairid > 64)
			continue;

		
		const auto& player = memory.Read<std::uintptr_t>(client + offset::dwEntityList + (crosshairid - 1) * 0x10);



		// skip if player is dead
		if (!memory.Read<std::int32_t>(player + offset::m_iHealth))
			continue;


		// skip if player is on our team 
		if (memory.Read<std::int32_t>(player + offset::m_iTeamNum) ==
			memory.Read<std::int32_t>(localPlayer + offset::m_iTeamNum))
			continue;

		memory.Write<std::uintptr_t>(client + offset::dwForceAttack, 6);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		memory.Write<std::uintptr_t>(client + offset::dwForceAttack, 4);






	}




	return 0;
	}