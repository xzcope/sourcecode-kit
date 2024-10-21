#define IMGUI_DEFINE_MATH_OPERATORS
#include "overlay.hpp"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_toggle.h"
#include "imgui/imgui_impl_dx11.h"
#include <dwmapi.h>
#include "../../sounds.h"
#include "../../roblox/esp/esp.hpp"
#include "../../roblox/globals/globals.hpp"
#include "../roblox/Class/playerClass.h"

#include <vector>
#include <thread>
#include <filesystem>
#include <windows.h>
#include <chrono>
#include <ctime>
#include <d3d11.h>
#include <tchar.h>
#include "../font.h"
#include "../texture.h"
#include "../../utils/overlay/imgui/imgui_edited.hpp"
#include "../../utils/overlay/imgui/imgui_freetype.h"
#include "../../utils/overlay/imgui/imgui.h"

#include "ckeybind/keybind.hpp"
#include "../skcrypt/skStr.hpp"
#include "../xorstr/xorstr.hpp"
#include "../configs/configs.hpp"
#include "../json/json.hpp"

#include <iostream>
#include <algorithm>
#include <set>
#include <deque>
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// Rest of your code goes here


#ifndef _MIN_MUL
#define _MIN_MUL 0.00
#define _MAX_MUL 0.50
#endif

#ifndef _MIN_DIV
#define _MIN_DIV 0.00
#define _MAX_DIV 15.00
#endif

bool info_bar = true;

const char* cheat_name = "d3p0s1t";
const char* game_status = "Roblox";
const char* developer = "";
const char* role = "user";
const char* fps = "1fps";
const char* ping = "4555ms";
const char* world_time = "12:20am";

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

static int notify_select = 0;
const char* notify_items[2]{ "Circle", "Line" };

namespace texture
{
	ID3D11ShaderResourceView* esp_preview = nullptr;
	ID3D11ShaderResourceView* dick = nullptr;

}
float calc_dist_3d_x(chaos::roblox::vector3_t first, chaos::roblox::vector3_t sec)
{
	return sqrt((first.x - sec.x) * (first.x - sec.x) + (first.y - sec.y) * (first.y - sec.y) + (first.z - sec.z) * (first.z - sec.z));
}

chaos::roblox::vector3_t vector_sub_x(chaos::roblox::vector3_t one, chaos::roblox::vector3_t two)
{
	return { one.x - two.x, one.y - two.y, one.z - two.z };
}

float vector3_mag_x(chaos::roblox::vector3_t vector)
{
	return sqrtf((vector.x * vector.y) + (vector.y * vector.y) + (vector.z * vector.z));
}


namespace vector_ops_x {
	float distance(const chaos::roblox::vector3_t& a, const chaos::roblox::vector3_t& b) {
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
	}

	chaos::roblox::vector3_t subtract(const chaos::roblox::vector3_t& a, const chaos::roblox::vector3_t& b) {
		return { a.x - b.x, a.y - b.y, a.z - b.z };
	}

	float magnitude(const chaos::roblox::vector3_t& v) {
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}
}

namespace esp_preview
{
	bool money = true;
	bool nickname = true;
	bool weapon = true;
	bool zoom = true;

	bool c4 = true;
	bool HP_line = true;
	bool hit = true;
	bool box = true;
	bool bomb = true;

	static float box_color[4] = { 37 / 255.f, 37 / 255.f, 47 / 255.f, 1.f };
	static float nick_color[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
	static float money_color[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
	static float zoom_color[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
	static float c4_color[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
	static float bomb_color[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
	static float hp_color[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
	static float hp_line_color[4] = { 112 / 255.f, 109 / 255.f, 214 / 255.f, 1.f };
	static float weapon_color[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
	static float hit_color[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };

	int hp = 85;
}

int page = 0;

static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;
bool enable_preview = false;

using namespace ImGui;

DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;



void CricleProgress(const char* name, float progress, float max, float radius, const ImVec2& size)
{
	const float tickness = 3.f;
	static float position = 0.f;

	position = progress / max * 6.28f;

	ImGui::GetForegroundDrawList()->PathClear();
	ImGui::GetForegroundDrawList()->PathArcTo(GetCursorScreenPos() + size, radius, 0.f, 2.f * IM_PI, 120.f);
	ImGui::GetForegroundDrawList()->PathStroke(ImGui::GetColorU32(c::elements::background_widget), 0, tickness);

	ImGui::GetForegroundDrawList()->PathClear();
	ImGui::GetForegroundDrawList()->PathArcTo(GetCursorScreenPos() + size, radius, IM_PI * 1.5f, IM_PI * 1.5f + position, 120.f);
	ImGui::GetForegroundDrawList()->PathStroke(ImGui::GetColorU32(c::accent), 0, tickness);

	int procent = progress / (int)max * 100;

	std::string procent_str = std::to_string(procent) + "%";

}


chaos::roblox::instance_t FindPlayerByName(const std::string& name) {

	auto players = globals::players;

	for (auto& player : players.children()) {
		if (player.name() == name) {
			return player;
		}
	}
	return chaos::roblox::instance_t();
}

struct Notification {
	int id;
	std::string message;
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
};

class NotificationSystem {
public:
	NotificationSystem() : notificationIdCounter(0) {}

	void AddNotification(const std::string& message, int durationMs) {
		auto now = std::chrono::steady_clock::now();
		auto endTime = now + std::chrono::milliseconds(durationMs);
		notifications.push_back({ notificationIdCounter++, message, now, endTime });
	}

	void DrawNotifications() {
		auto now = std::chrono::steady_clock::now();

		std::sort(notifications.begin(), notifications.end(),
			[now](const Notification& a, const Notification& b) -> bool {
				float durationA = std::chrono::duration_cast<std::chrono::milliseconds>(a.endTime - a.startTime).count();
				float elapsedA = std::chrono::duration_cast<std::chrono::milliseconds>(now - a.startTime).count();
				float percentageA = (durationA - elapsedA) / durationA;

				float durationB = std::chrono::duration_cast<std::chrono::milliseconds>(b.endTime - b.startTime).count();
				float elapsedB = std::chrono::duration_cast<std::chrono::milliseconds>(now - b.startTime).count();
				float percentageB = (durationB - elapsedB) / durationB;

				return percentageA < percentageB;
			}
		);

		int currentNotificationPosition = 0;

		for (auto& notification : notifications) {
			if (now < notification.endTime) {
				float duration = std::chrono::duration_cast<std::chrono::milliseconds>(notification.endTime - notification.startTime).count();
				float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - notification.startTime).count();
				float percentage = (duration - elapsed) / duration * 100.0f;

				ShowNotification(currentNotificationPosition, notification.message, percentage);
				currentNotificationPosition++;
			}
		}

		notifications.erase(std::remove_if(notifications.begin(), notifications.end(),
			[now](const Notification& notification) { return now >= notification.endTime; }),
			notifications.end());
	}

private:
	std::vector<Notification> notifications;
	int notificationIdCounter;

	void ShowNotification(int position, const std::string& message, float percentage) {

		float duePercentage = 100.0f - percentage;
		float alpha = percentage > 10.0f ? 1.0f : percentage / 10.0f;
		GetStyle().WindowPadding = ImVec2(15, 10);

		ImGui::SetNextWindowPos(ImVec2(duePercentage < 15.f ? duePercentage : 15.f, 15 + position * 90));

		ImGui::Begin(("##NOTIFY" + std::to_string(position)).c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);

		ImVec2 pos = ImGui::GetWindowPos(), spacing = ImGui::GetStyle().ItemSpacing, region = ImGui::GetContentRegionMax();

		GetBackgroundDrawList()->AddRectFilledMultiColor(pos, pos + region, ImGui::GetColorU32(c::background::filling, alpha), ImGui::GetColorU32(c::accent, 0.01f), ImGui::GetColorU32(c::accent, 0.01f), ImGui::GetColorU32(c::background::filling, alpha), c::elements::rounding);
		GetBackgroundDrawList()->AddRectFilled(pos, pos + region, ImGui::GetColorU32(c::background::filling, 0.4f), c::elements::rounding);
		GetBackgroundDrawList()->AddRect(pos, pos + region, ImGui::GetColorU32(c::background::stroke, alpha), c::elements::rounding);

		if (notify_select == 0)
			CricleProgress("##NOTIFY", percentage, 100, 7.f, ImVec2(GetContentRegionMax().x - 40, 11));
		if (notify_select == 1)
			GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, region.y - 3), pos + ImVec2(region.x * (duePercentage / 100.0f), region.y), ImGui::GetColorU32(c::accent, alpha), c::elements::rounding);

		ImGui::PushFont(font::lexend_bold);
		ImGui::TextColored(ImColor(GetColorU32(c::accent, alpha)), "%s", "[d3p0s1t]");
		ImGui::TextColored(ImColor(GetColorU32(c::elements::text_active, alpha)), "%s", message.c_str());
		ImGui::Dummy(ImVec2(CalcTextSize(message.c_str()).x + 15, 5));
		PopFont();

		ImGui::End();
	}
};

struct Notification_text {
	std::string text;
	std::chrono::time_point<std::chrono::steady_clock> start_time;
	float duration;
	int type;

	bool is_removing; 
	std::chrono::time_point<std::chrono::steady_clock> remove_start_time; 
	float alpha; 
};

std::deque<Notification_text> notifications;

void chaos::roblox::AddNotification(const std::string& text, float duration, int type) {
	notifications.push_back({ text, std::chrono::steady_clock::now(), duration, type, false, std::chrono::steady_clock::time_point() });
}


void DrawNotifications() {
	if (notifications.empty()) return;

	POINT cursor_pos;
	GetCursorPos(&cursor_pos);
	HWND hwndRoblox = FindWindowA(NULL, "Roblox");
	ScreenToClient(hwndRoblox, &cursor_pos);

	ImVec2 crosshair_position = ImVec2(static_cast<float>(cursor_pos.x), static_cast<float>(cursor_pos.y));
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	float y_offset = 130.0f;

	auto now = std::chrono::steady_clock::now();
	for (auto it = notifications.begin(); it != notifications.end();) {
		auto elapsed = std::chrono::duration<float>(now - it->start_time).count();


		if (elapsed > it->duration) {
			it = notifications.erase(it);
			continue;
		}

		float alpha = 1.0f;
		if (elapsed < 0.2f * it->duration) {
			alpha = elapsed / (0.2f * it->duration); 
		}
		else if (elapsed > 0.8f * it->duration) {
			alpha = 1.0f - (elapsed - 0.8f * it->duration) / (0.2f * it->duration); 
		}


		std::string prefix, suffix, postfix = "] -> ";
		if (it->type == 1) {
			prefix = "d3p0s1t";
			suffix = ".cc";
		}
		else if (it->type == 2) {
			prefix = "[Watchdog";
			suffix = "";
		}

		std::string notification_text = prefix + suffix + postfix + it->text;
		ImGui::PushFont(font::monospace);

		ImVec2 text_size = ImGui::CalcTextSize(notification_text.c_str());
		ImVec2 text_pos = crosshair_position + ImVec2(-text_size.x / 2, y_offset);

		ImColor outline_color = ImColor(0, 0, 0, static_cast<int>(255 * alpha));
		ImColor text_color = ImColor(255, 255, 255, static_cast<int>(255 * alpha));
		ImColor purple_color = ImColor(128, 0, 128, static_cast<int>(255 * alpha));

		float text_x = text_pos.x;
		float outline_thickness = 1.0f;

		auto HelpANiggaOut = [&](ImVec2 pos, ImColor color, const std::string& text) {
			drawList->AddText(pos + ImVec2(-outline_thickness, -outline_thickness), outline_color, text.c_str());
			drawList->AddText(pos + ImVec2(outline_thickness, -outline_thickness), outline_color, text.c_str());
			drawList->AddText(pos + ImVec2(-outline_thickness, outline_thickness), outline_color, text.c_str());
			drawList->AddText(pos + ImVec2(outline_thickness, outline_thickness), outline_color, text.c_str());
			drawList->AddText(pos + ImVec2(0, -outline_thickness), outline_color, text.c_str());
			drawList->AddText(pos + ImVec2(0, outline_thickness), outline_color, text.c_str());
			drawList->AddText(pos + ImVec2(-outline_thickness, 0), outline_color, text.c_str());
			drawList->AddText(pos + ImVec2(outline_thickness, 0), outline_color, text.c_str());
			drawList->AddText(pos, color, text.c_str());
			};

		HelpANiggaOut(ImVec2(text_x, text_pos.y), text_color, prefix);  // prefix
		text_x += ImGui::CalcTextSize(prefix.c_str()).x;

		ImColor suffix_color = (it->type == 1) ? purple_color : text_color;
		HelpANiggaOut(ImVec2(text_x, text_pos.y), suffix_color, suffix);  // suffix
		text_x += ImGui::CalcTextSize(suffix.c_str()).x;

		HelpANiggaOut(ImVec2(text_x, text_pos.y), text_color, postfix);  // postfix
		text_x += ImGui::CalcTextSize(postfix.c_str()).x;

		HelpANiggaOut(ImVec2(text_x, text_pos.y), text_color, it->text);  // main text

		y_offset += text_size.y + 5;
		++it;

		ImGui::PopFont();
	}
}

NotificationSystem notificationSystem;


ID3D11Device* chaos::utils::overlay::d3d11_device = nullptr;

ID3D11DeviceContext* chaos::utils::overlay::d3d11_device_context = nullptr;

IDXGISwapChain* chaos::utils::overlay::dxgi_swap_chain = nullptr;

ID3D11RenderTargetView* chaos::utils::overlay::d3d11_render_target_view = nullptr;

static const char* combo_items_4[5] = { ("Head"), ("UpperTorso"), ("HumanoidRootPart"), ("LowerTorso"), ("Torso (R6)") };
static const char* macro_items[2] = { ("First Person"), ("I & O Keys") };
static const char* box_items_3[3] = { ("2D Box"), ("3D Box"), ("Dynamic 3D Box")};
static const char* shake_items[2] = { ("X / Y"), ("Multiplication") };
static const char* aimbot_items[2] = { ("Mouse"), ("Camera")};

static const char* pred_items[2] = { ("Division"), ("Multiplication") };
static const char* fovitems[2] = { ("Normal Fov"), ("LGBTQSENSE") };
static const char* head_dotx[2] = { ("Circle"), ("Filled") };
static const char* thread_type[2] = { ("Milliseconds"), ("Microseconds") };
static const char* smooth_typessex[5] = { ("Linear"), ("Bounce"), ("Elastic"), ("Quadratic"), ("Decay")};

static const char* prediction_methods[2] = { ("X / Y"), ("Multiplication") };

static const char* hit_sounds[4] = { ("Neverlose"), ("Skeet"), ("Fuck"), ("Senpai")};


std::string getCurrentTime() {
	auto now = std::chrono::system_clock::now();
	auto now_time_t = std::chrono::system_clock::to_time_t(now);
	auto now_tm = std::localtime(&now_time_t);

	std::stringstream ss;
	ss << std::put_time(now_tm, "%H:%M:%S");
	return ss.str();
}

std::vector<std::string> fetchPlayerNames() {
	std::vector<std::string> playerNames;

	auto players = globals::players;

	for (auto& player : playerList) {
		if (!player.playerInstance.self) continue;

		auto playerName = player.playerInstance.name();
		playerNames.push_back(playerName);
	}

	return playerNames;
}


bool Keybind(CKeybind* keybind, const ImVec2& size_arg = ImVec2(0, 0), bool clicked = false, ImGuiButtonFlags flags = 0)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(keybind->get_name().c_str());
	const ImVec2 label_size = ImGui::CalcTextSize(keybind->get_name().c_str(), NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) &&
		style.FramePadding.y <
		window->DC.CurrLineTextBaseOffset)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(
		size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (g.CurrentItemFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

	bool value_changed = false;
	int key = keybind->key;

	auto io = ImGui::GetIO();

	std::string name = keybind->get_key_name();

	if (keybind->waiting_for_input)
		name = "waiting";

	if (ImGui::GetIO().MouseClicked[0] && hovered)
	{
		if (g.ActiveId == id)
		{
			keybind->waiting_for_input = true;
		}
	}
	else if (ImGui::GetIO().MouseClicked[1] && hovered)
	{
		ImGui::OpenPopup(keybind->get_name().c_str());
	}
	else if (ImGui::GetIO().MouseClicked[0] && !hovered)
	{
		if (g.ActiveId == id)
			ImGui::ClearActiveID();
	}

	if (keybind->waiting_for_input)
	{
		if (ImGui::GetIO().MouseClicked[0] && !hovered)
		{
			keybind->key = VK_LBUTTON; 

			ImGui::ClearActiveID();
			keybind->waiting_for_input = false;
		}
		else
		{
			if (keybind->set_key())
			{
				ImGui::ClearActiveID();
				keybind->waiting_for_input = false;
			}
		}
	}
	ImGui::PushFont(font::lexend_regular);

	ImVec4 textcolor = ImLerp(ImVec4(201 / 255.f, 204 / 255.f, 210 / 255.f, 1.f), ImVec4(1.0f, 1.0f, 1.0f, 1.f), 1.f);


	window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(33 / 255.0f, 33 / 255.0f, 33 / 255.0f, 0.5f), 2.f);


	ImVec2 text_pos = bb.Min + ImVec2(
		size_arg.x / 2 - ImGui::CalcTextSize(name.c_str()).x / 2,
		size_arg.y / 2 - ImGui::CalcTextSize(name.c_str()).y / 2);

	window->DrawList->AddText(text_pos, ImGui::GetColorU32(textcolor), name.c_str());

	if (ImGui::BeginPopup(keybind->get_name().c_str(), 0))
	{
		ImGui::BeginGroup();
		if (edited::Selectable("Hold", keybind->type == CKeybind::HOLD))
			keybind->type = CKeybind::HOLD;
		if (edited::Selectable("Toggle", keybind->type == CKeybind::TOGGLE))
			keybind->type = CKeybind::TOGGLE;
		ImGui::EndGroup();
		ImGui::EndPopup();
	}

	PopFont();
	return pressed;
}


bool chaos::utils::overlay::fullsc(HWND windowHandle)
{
	MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
	if (GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &monitorInfo))
	{
		RECT windowRect;
		if (GetWindowRect(windowHandle, &windowRect))
		{
			return windowRect.left == monitorInfo.rcMonitor.left
				&& windowRect.right == monitorInfo.rcMonitor.right
				&& windowRect.top == monitorInfo.rcMonitor.top
				&& windowRect.bottom == monitorInfo.rcMonitor.bottom;
		}
	}
}

std::string GetConfigFolderPath()
{
	std::string configFolderPath = chaos::utils::appdata_path() + "\\d3p0s1t\\configs";

	if (!std::filesystem::exists(configFolderPath))
	{
		std::filesystem::create_directory(configFolderPath);
	}

	return configFolderPath;
}

bool isValid(const std::string& str) {
	bool startsWithNumber = std::isdigit(str[0]);
	bool containsSymbol = false;

	for (char ch : str) {
		if (!std::isalnum(ch) && ch != '_') {
			containsSymbol = true;
			break;
		}
	}

	return startsWithNumber || containsSymbol;
}


void DisplayInstanceTree(chaos::roblox::instance_t instance, std::string parentPath = "NULL")
{
	std::string instancePath;
	std::string istanceName = instance.name();

	if (parentPath == "NULL") {
		instancePath = "game:GetService(\"";
	}
	else {
		instancePath = (parentPath == "game:GetService(\"") ? parentPath + istanceName + "\")" : (!isValid(istanceName)) ? parentPath + "[\"" + istanceName + "\"]" : parentPath + "." + instance.name();
	}

	for (auto& child : instance.children())
	{
		std::string childName = child.name();
		std::string childPath = instancePath + "[\"" + childName + "\"]";

		ImGui::PushID(childPath.c_str());
		bool isNodeOpen = ImGui::TreeNodeEx(childName.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);

		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(1))
		{
			ImGui::OpenPopup("Context Menu");
		}

		if (ImGui::BeginPopup("Context Menu"))
		{
			if (ImGui::MenuItem("Copy Path"))
			{
				ImGui::SetClipboardText(childPath.c_str());
			}
			ImGui::EndPopup();
		}

		if (isNodeOpen)
		{
			DisplayInstanceTree(child, instancePath);
			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}

static char text[999] = "";
char configname[100];

bool chaos::utils::overlay::init = false;

bool if_first = false;

bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f)
{
	ImGuiStyle& style = ImGui::GetStyle();

	float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	return ImGui::Button(label);
}

std::map<std::string, float> previousHealths;

void CheckHealthAndNotify() {
	if (!globals::friendlynotifier) {
		return;
	}

	printf("Friendly notifier status: %s\n", globals::friendlynotifier ? "Enabled" : "Disabled");

	for (const auto& playerName : globals::aimwhitelistedPlayers) {
		auto playerInstance = FindPlayerByName(playerName);
		printf("Retrieving player: %s - Found: %s\n", playerName.c_str(), playerInstance.self ? "Yes" : "No");

		if (playerInstance.self) {
			auto humanoid = playerInstance.find_first_child("Humanoid");
			printf("Retrieving humanoid for player: %s - Found: %s\n", playerName.c_str(), humanoid.self ? "Yes" : "No");

			if (humanoid.self) {
				float currentHealth = humanoid.get_health();
				printf("Direct Health for %s: %f\n", playerName.c_str(), currentHealth);

				if (previousHealths.find(playerName) != previousHealths.end()) {
					float previousHealth = previousHealths[playerName];
					printf("Previous Health for %s: %f\n", playerName.c_str(), previousHealth);

					if (currentHealth < previousHealth) {
						float damageTaken = previousHealth - currentHealth;
						char notificationMessage[256];
						snprintf(notificationMessage, sizeof(notificationMessage), "%s took %.0f damage.", playerName.c_str(), damageTaken);
						printf("Sending notification: %s\n", notificationMessage);
						notificationSystem.AddNotification(notificationMessage, 1000);
					}
				}
				else {
					printf("No previous health record found for player: %s\n", playerName.c_str());
				}
				previousHealths[playerName] = currentHealth;
			}
		}
	}
}



static HWND Background() {
	Sleep(50);
	 return GetForegroundWindow();
}


bool chaos::utils::overlay::render()
{
	ImGui_ImplWin32_EnableDpiAwareness();

	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpfnWndProc = window_proc;
	wc.lpszClassName = TEXT("feshgvgavshfagfvd");
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&wc);
	const HWND hw = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, wc.lpszClassName, TEXT("sdwdadsgf"),
		WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);

	SetLayeredWindowAttributes(hw, 0, 255, LWA_ALPHA);
	const MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(hw, &margin);

	if (!create_device_d3d(hw))
	{
		cleanup_device_d3d();
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return false;
	}

	ShowWindow(hw, SW_SHOW);
	UpdateWindow(hw);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui::GetIO().IniFilename = nullptr;

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 originalButtonColor = style.Colors[ImGuiCol_Button];

	ImGuiIO& io = ImGui::GetIO();



	ImFontConfig cfg;

	cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;


	//	font::nigger = io.Fonts->AddFontFromMemoryTTF(nigga1, sizeof(nigga1), 14.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

	font::lexend_general_bold = io.Fonts->AddFontFromMemoryTTF(lexend_bold, sizeof(lexend_bold), 18.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	font::lexend_bold = io.Fonts->AddFontFromMemoryTTF(lexend_regular, sizeof(lexend_regular), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	font::lexend_regular = io.Fonts->AddFontFromMemoryTTF(lexend_regular, sizeof(lexend_regular), 14.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	font::icomoon = io.Fonts->AddFontFromMemoryTTF(icomoon, sizeof(icomoon), 20.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());


	font::icomoon_widget = io.Fonts->AddFontFromMemoryTTF(icomoon_widget, sizeof(icomoon_widget), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	font::icomoon_widget2 = io.Fonts->AddFontFromMemoryTTF(icomoon, sizeof(icomoon), 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	font::monospace = io.Fonts->AddFontFromMemoryTTF(monospace, sizeof(monospace), 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	font::roboto_regular = io.Fonts->AddFontFromMemoryTTF(roboto_regular, sizeof(roboto_regular), 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

	D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
	if (texture::esp_preview == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3d11_device, esp_preview1, sizeof(esp_preview1), &info, pump, &texture::esp_preview, 0);
	if (texture::dick == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3d11_device, dick1, sizeof(dick1), &info, pump, &texture::dick, 0);



	ImGui_ImplWin32_Init(hw);
	ImGui_ImplDX11_Init(d3d11_device, d3d11_device_context);

	const ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	init = true;

	bool draw = false;
	bool done = false;
	bool donesp = false;
	
	HWND roblox = globals::window_handle;
	HWND GetBackground;


	while (!done)
	{
		

		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}
		if (done) break;

		move_window(hw);

		if (GetAsyncKeyState(VK_INSERT) & 1)
			draw = !draw;
		
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			ImGuiStyle* style = &ImGui::GetStyle();

			notificationSystem.DrawNotifications();
			DrawNotifications();

			style->WindowPadding = ImVec2(0, 0);
			style->ItemSpacing = ImVec2(10, 10);
			style->WindowBorderSize = 0;
			style->ScrollbarSize = 8.f;

			static float color[4] = { 0 / 255.f, 0 / 255.f, 255 / 255.f, 1.f };
			c::accent = { color[0], color[1], color[2], 1.f };

			GetBackground = GetForegroundWindow();
			

			if (GetBackground  == roblox || GetBackground == hw)
			{

				ImGui::Begin("overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
				{
					chaos::roblox::hook_esp();
				}
			
				if (draw)
				{

					SetNextWindowSize(c::background::size);
					ImGui::Begin("IMGUI!", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);
					{
						const ImVec2& pos = GetWindowPos();
						const ImVec2& region = GetContentRegionMax();
						const ImVec2& spacing = style->ItemSpacing;

						GetBackgroundDrawList()->AddRectFilled(pos, pos + c::background::size, GetColorU32(c::background::filling), c::background::rounding);
						GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(200.f, c::background::size.y), GetColorU32(c::tab::border), c::background::rounding, ImDrawFlags_RoundCornersLeft);
						GetBackgroundDrawList()->AddLine(pos + ImVec2(200.f, 0.f), pos + ImVec2(200.f, c::background::size.y), GetColorU32(c::background::stroke), 1.f);

						GetBackgroundDrawList()->AddRect(pos, pos + c::background::size, GetColorU32(c::background::stroke), c::background::rounding);

						SetCursorPos(ImVec2(61, 20));
						ImGui::PushFont(font::lexend_general_bold);
						ImGui::TextColored(ImColor(GetColorU32(c::accent)), TEXT("nevermiss.cc"));
						PopFont();


						SetCursorPos(ImVec2(68, 35));
						ImGui::PushFont(font::lexend_regular);
						ImGui::TextColored(ImColor(GetColorU32(c::accent)), TEXT(".gg/AMPR4d2Dk6"));
						PopFont();


						SetCursorPos(ImVec2(60, 45));
						ImGui::PushFont(font::lexend_regular);
						ImGui::TextColored(ImColor(GetColorU32(c::accent)), TEXT("Build: 1.0.0"));
						PopFont();

						SetCursorPos(ImVec2(15, 100));
						ImGui::PushFont(font::lexend_bold);
						ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), TEXT("        BETA"));
						PopFont();


						SetCursorPos(ImVec2(10, 150));
						ImGui::BeginGroup();


						std::vector<std::vector<std::string>> tab_columns = {
											{ "c", "b", "f","o", "e", "a", },
											{ "Aimbot", "Visuals","World", "Configs", "Settings", "Misc", },
											{ "https://discord.gg/AMPR4d2Dk6", "https://discord.gg/AMPR4d2Dk6", "https://discord.gg/AMPR4d2Dk6","https://discord.gg/AMPR4d2Dk6", "https://discord.gg/AMPR4d2Dk6", "https://discord.gg/AMPR4d2Dk6",   },
											{ "https://discord.gg/AMPR4d2Dk6", "https://discord.gg/AMPR4d2Dk6", "https://discord.gg/AMPR4d2Dk6",  "https://discord.gg/AMPR4d2Dk6", "https://discord.gg/AMPR4d2Dk6", "https://discord.gg/AMPR4d2Dk6", }
						};

						for (int i = 0; i < tab_columns[0].size(); ++i) {

							if (edited::Tab(page == i, tab_columns[0][i].c_str(), tab_columns[1][i].c_str(), tab_columns[2][i].c_str(), ImVec2(180, 50))) {
								notificationSystem.AddNotification(tab_columns[3][i], 1000);
								page = i;
							}

						}

						EndGroup();

						tab_alpha = ImLerp(tab_alpha, (page == active_tab) ? 1.f : 0.f, 15.f * ImGui::GetIO().DeltaTime);
						if (tab_alpha < 0.01f && tab_add < 0.01f) active_tab = page;

						ImGui::SetCursorPos(ImVec2(200, 100 - (tab_alpha * 100)));

						ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style->Alpha);
						{

							if (active_tab == 0)
							{
								edited::BeginChild("##Container0", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
								{
									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Aiming");

									edited::Checkbox("Aimbot", "Enables Aimbot", &globals::aimbot);
									edited::Checkbox("Prediction", "Enables Prediction", &globals::prediction);
									edited::Combo("Method", "Select type of aimbot", &globals::aimtype, aimbot_items, IM_ARRAYSIZE(aimbot_items), 2);
									edited::Checkbox("Spectate Targer", "Activates Spectating locked target", &globals::spectatelocked);
									edited::Checkbox("Wallbang", "Gangbang", &globals::wallbang);

									edited::Checkbox("Shake", "Enables Shake", &globals::shake);
									//edited::Checkbox("Smoothness", "Enables Smoothness", &globals::smoothness);
									edited::Checkbox("Range", "Enables aimbot range", &globals::aimbot_range_enable);

									if (globals::aimbot_range_enable)
										edited::SliderInt("Aimbot Range", "Changes aimbot range not that big deal", &globals::aimbot_range, 1.0, 100.0, "%d%%");

									if (globals::shake)
										edited::Combo("Select Type", "Select type of shake", &globals::shaketype, shake_items, IM_ARRAYSIZE(shake_items), 2);

									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "General");

									edited::Checkbox("Auto Resolver", "Enables Auto Resolver", &globals::autoresolve);

									if (globals::autoresolve)
										edited::SliderFloat("Threshold", "Velocity Threshold", &globals::velocity_threshold, 1, 100, "%.1f");

									edited::Checkbox("Resolver", "Enables Resolver (manual)", &globals::resolver);
									edited::Checkbox("Deadzone", "Enables Deadzone", &globals::deadzone);
									edited::Checkbox("Disable outside of Fov", "Enables Disable outside of fov", &globals::disable_outside_fov);
								}
								edited::EndChild();

								SameLine(0, 0);

								edited::BeginChild("##Container1", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
								{
									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Options");

									static int key1 = 0;
									double _pred_min = globals::prediction_method == 0 ? _MIN_DIV : _MIN_MUL;
									double _pred_max = globals::prediction_method == 0 ? _MAX_DIV : _MAX_MUL;

									if (globals::prediction_method == 0) {
										if (globals::main_prediction < _MIN_DIV)
											globals::main_prediction = globals::main_close_prediction = globals::main_mid_prediction = globals::main_far_prediction = _MIN_DIV;

										if (globals::prediction_x < _MIN_DIV)
											globals::prediction_x = globals::close_prediction_x = globals::mid_prediction_x = globals::far_prediction_x = _MIN_DIV;

										if (globals::prediction_y < _MIN_DIV)
											globals::prediction_y = globals::close_prediction_y = globals::mid_prediction_y = globals::far_prediction_y = _MIN_DIV;
									}
									else {
										if (globals::main_prediction > _MAX_MUL)
											globals::main_prediction = globals::main_close_prediction = globals::main_mid_prediction = globals::main_far_prediction = _MAX_MUL;

										if (globals::prediction_x > _MAX_MUL)
											globals::prediction_x = globals::close_prediction_x = globals::mid_prediction_x = globals::far_prediction_x = _MAX_MUL;

										if (globals::prediction_y > _MAX_MUL)
											globals::prediction_y = globals::close_prediction_y = globals::mid_prediction_y = globals::far_prediction_y = _MAX_MUL;
									}


									Keybind(&globals::aimbotkey, ImVec2(60, 20));

									edited::Combo("Aimpart", "Aim part", &globals::aimpart, combo_items_4, IM_ARRAYSIZE(combo_items_4), 5);

									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Field OF View");

									edited::Checkbox("Draw Fov", "Draws Field OF View", &globals::fov_on);


									if (globals::fov_on)
										edited::Combo("Fov", "Fov Type", &globals::fovtype, fovitems, IM_ARRAYSIZE(fovitems), 2);

									if (globals::fovtype == 0) {
										edited::Checkbox("Fill Fov +", "Fills Field of View +", &globals::fov_filled);
										edited::Checkbox("Fov Outline", "Fov Outline", &globals::outlinefov);
										edited::Separator_line();

										edited::Checkbox("Sonar", "Somewhat of a pulse effect for FOV", &globals::sonar);
										edited::SliderFloat("Sonar Expansion Rate", "Changes the expansion rate of sonar", &globals::sonar_expansion_rate, 0.1f, 10.f, "%.1f");
										edited::Separator_line();

									}
									else if (globals::fovtype == 1)
										edited::Checkbox("Fill Fov Gay", "Fills Field of View but Gay", &globals::rainbowfovfilled);

									if (globals::deadzone)
										edited::Checkbox("Draw Deadzone", "Draws Deadzone", &globals::show_deadzone);

									if (globals::deadzone)
										edited::Checkbox("Fill Deadzone", "fill Deadzone", &globals::deadzone_fov_filled);

									edited::SliderInt("FOV", "Changes Field OF View", &globals::fov, 1, 1000, "%d%%");

									if (globals::deadzone)
										edited::SliderInt("Deadzone", "Changes Field OF View", &globals::deadzone_value, 1, 1000, "%d%%");


									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Smoothness");

									if (globals::smoothness) {
										edited::Combo("Type", "Smoothness Type", &globals::smooth_type, smooth_typessex, IM_ARRAYSIZE(smooth_typessex), 5);

										if (globals::smooth_type == 1) {
											edited::SliderFloat("Frequency", "changes frequency of bounce", &globals::bounce_frequency, 0.01, 30.0, "%.1f");
											edited::SliderFloat("Decay", "changes decay of bounce", &globals::bounce_decay, 0.01, 10.0, "%.1f");
										}
										if (globals::smooth_type == 2) {
											edited::SliderFloat("Frequency", "changes frequency of elastic", &globals::elactic_frequency, 0.01, 30.0, "%.1f");
											edited::SliderFloat("Decay", "changes decay of elastic", &globals::elactic_decay, 0.01, 10.0, "%.1f");
										}
										if (globals::smooth_type == 4) {
											edited::SliderFloat("Decay", "changes decay value", &globals::decay, 0.01, 10.0, "%.1f");
										}
									}

									edited::SliderFloat("Smoothness X", "Makes aimbot more smooth by X", &globals::smoothness_x, 1.0, 30.0, "%.1f");
									edited::SliderFloat("Smoothness Y", "Makes aimbot more smooth by Y", &globals::smoothness_y, 1.0, 30.0, "%.1f");

									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Prediction");


									if (globals::prediction) {
										edited::Separator_line();

										edited::Checkbox("Auto Prediction", "Automatically calculates the prediction", &globals::autopred);
										edited::Checkbox("Separate Prediction", "Separates the prediction XY into sliders", &globals::separate_predictions);
										edited::Combo("Prediction Method", "Select the method of prediction", &globals::prediction_method, pred_items, IM_ARRAYSIZE(pred_items), 2);
										edited::Separator_line();

									}


									if (globals::separate_predictions) 
									{
										edited::SliderFloat("Prediction X", "Predicts player move by X", &globals::prediction_x, _pred_min, _pred_max, "%.0005f");
										edited::SliderFloat("Prediction Y", "Predicts player move by Y", &globals::prediction_y, _pred_min, _pred_max, "%.0005f");
									}
									else 
									{
										edited::SliderFloat("Prediction", "Predicts player move by all axis", &globals::main_prediction, _pred_min, _pred_max, "%.0005f");
									}
								
							

									if (globals::shaketype == 0) {
										ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Shake");

										edited::SliderFloat("Shake X", "shakes aimbot by X", &globals::shake_x, 0.1, 10.0, "%.1f");
										edited::SliderFloat("Shake Y", "shakes aimbot by Y ", &globals::shake_y, 0.1, 10.0, "%.1f");
									}
									else if (globals::shaketype == 1) {
										ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Shake");

										edited::SliderFloat("Shake value", "shakes aimbot", &globals::shake_value, 0.1, 10.0, "%.1f");
									}

									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Sensitivity");


									edited::SliderFloat("Sensitivity", "makes aimbot more hard / smooth", &globals::sensitivity, 0.1, 5.0, "%.1f");
									edited::Combo("Thread type", "Changes aimbot thread type", &globals::threadtype, thread_type, IM_ARRAYSIZE(thread_type), 2);




								}
								edited::EndChild();
							}
							else if (active_tab == 1)
							{
								edited::BeginChild("##Container0", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
								{
									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Visuals");

									edited::Checkbox("Enable", "Enables Esp", &globals::esp);

									//edited::Checkbox("Visualise Target", "Esp only on locked person", &globals::visualisetarget);

									//if (globals::visualisetarget) {

									//	edited::Checkbox("Target Mark", "Display's Target Status", &globals::targetmark);

									//}

									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Main");

									edited::Checkbox("Box", "Box Esp", &globals::box);

									if (globals::box) {


										edited::Combo("Box Type", "Box Type", &globals::boxtype, box_items_3, IM_ARRAYSIZE(box_items_3), 3);

									}

									if (globals::boxtype == 1) {
										edited::Checkbox("Rainbow", "Rainbow Corner Box", &globals::rainbowmode);

									}
									edited::Checkbox("Skeleton ESP", "Draw player bones", &globals::skeleton);

									edited::Checkbox("Outlines", "Draw outlines", &globals::outline);


									//edited::Checkbox("Draw Radar", "Radar esp", &globals::radar);


							       //edited::Checkbox("Fill Box", "Fills The box", &globals::filledbox);

								//edited::Checkbox("Fill Box rainbow", "Fills The box (rainbow)", &globals::rainbowbox);


									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Other Visuals");

									edited::Checkbox("Head Dot", "Draws dot to player head", &globals::head_dot);

									edited::Checkbox("3D Circle", "Draws 3d circle on player ground", &globals::threeCircle);

									
									if (globals::threeCircle) {

										edited::SliderFloat("3D Circle Y axis", "Changes Y axis", &globals::y_axis, -10.0, 13.0, "%.1f");

									}

									edited::Checkbox("Name Esp", "Draws player name", &globals::name_esp);

									edited::Checkbox("Distance Esp", "Draws distance between you and player", &globals::distance_esp);

								    edited::Checkbox("Tracers", "Draws tracer between ur mouse and player", &globals::tracers);

									edited::Checkbox("Health Info", "Draws player Health", &globals::healthinfo);

								    edited::Checkbox("Health Bar", "Draws Player health", &globals::healthbar);
									edited::Checkbox("Armor Bar", "Draws Player Armor", &globals::armorbar);


									edited::Separator_line();

									//edited::Checkbox("Crosshair", "Draws crosshair", &globals::crosshair);
								}
								edited::EndChild();

								SameLine(0, 0);

								edited::BeginChild("##Container1", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
								{

									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Customizibeable");

									edited::ColorEdit4("Box Color", "Changes box color", globals::box_color, picker_flags);
									edited::ColorEdit4("Filled Color", "Changes Fill Box color", globals::fill_color, picker_flags);
									edited::ColorEdit4("Name Color", "Changes Name Color", globals::name_color, picker_flags);
									edited::ColorEdit4("Tracer Color", "Changes Trace Color", globals::tracers_color, picker_flags);
									edited::ColorEdit4("Head Dot", "Changes Head Dot Color", globals::head_dot_color, picker_flags);

									if (globals::head_dot)
										edited::Combo("Head dot type", "Changes head dot type", &globals::head_dot_type, head_dotx, IM_ARRAYSIZE(head_dotx), 2);

									if (globals::targetmark) {
										ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Target Mark");

										edited::Checkbox("Lower Center", "reposition the target mark", &globals::lowerbottom);
										edited::Checkbox("Semi Transparent", "changes transparent of the target mark", &globals::transparent);
									}

									if (globals::fov_on) {
										ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Fov Colors");

										edited::ColorEdit4("Fov Color", "Changes Fov Color", globals::fov_color, picker_flags);
										edited::ColorEdit4("Fov Filled Color", "Changes Fov filled color", globals::fov_fill_color, picker_flags);

										if (globals::deadzone) {
											edited::ColorEdit4("Deadzone  Color", "Changes Deadzone Color", globals::deadzone_color, picker_flags);
											edited::ColorEdit4("Deadzone Fill Color", "Changes Deadzone Color", globals::deadzone_fill_color, picker_flags);
										}
									}



									if (globals::crosshair) {

										edited::Separator_line();

										edited::SliderFloat("Thickness", "Crosshair Thickness", &globals::thickness, 1, 10, "%.1f");
										edited::SliderFloat("Gap", "Crosshair Gap", &globals::gap, 1.0, 50.0, "%.1f");
										edited::SliderFloat("lenght", "Crosshair lenght", &globals::lenght, 1.0, 50.0, "%.1f");
									}
								}
								edited::EndChild();
							}
								else if (active_tab == 2)
								{
									edited::BeginChild("##Container0", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
									{

						
										ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Mods");

										edited::Checkbox("CSpeed", "Enables CFrame Speed", &globals::cframe_enable);
										edited::SliderFloat("Multipler", "Modifies CSpeed velocity", &globals::cframe_multiplier, 0.5f, 10, "%.01f");
										Keybind(&globals::cframekey, ImVec2(60, 20));

										//edited::Checkbox("Enables", "Enables Trigger Bot ", &globals::triggerbot);
										//edited::Checkbox("Prediction", "Enables Triggerbot prediction ", &globals::trigger_prediction);

										//Keybind(&globals::triggerbotkey, ImVec2(60, 20));

										if (globals::trigger_prediction) {
											edited::SliderFloat("Trigger Prediction X", "predicts player move by X", &globals::trigger_prediction_x, 0.10, 1.00, "%.0005f");
											edited::SliderFloat("Trigger Prediction Y", "predicts player move by Y", &globals::trigger_prediction_y, 0.10, 1.00, "%.0005f");
										}

										if (globals::triggerbot)
											edited::SliderInt("Delay", "Shoot delay", &globals::triggerbot_delay_ms, 1, 1000, "%d%%");

										if (globals::triggerbot)
											edited::SliderFloat("Triggerbot Range", "Detect range", &globals::trigger_range, 1.0, 100.0, "%.1f");
									}
									edited::EndChild();

									SameLine(0, 0);

									edited::BeginChild("##Container1", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
									{
										//ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "FlickBot");
										//edited::Checkbox("Enable", "Enables Flick bot ", &globals::flickbot);
										//edited::SliderFloat("Flick Speed", "changes flick speed", &globals::flickspeed, 0.1, 10.0, "%.1f");
										//edited::Checkbox("Auto Shoot", "Auto shoots after lock ", &globals::autoshoot);

										//Keybind(&globals::flickbotkey, ImVec2(60, 20));
									}
									edited::EndChild();
								}

							else if (active_tab == 3)
							{
								edited::BeginChild("##Container0", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
								{
									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Config");

									std::vector<std::string> configFiles;
									std::string configFolderPath = GetConfigFolderPath();
									for (auto file : std::filesystem::directory_iterator(configFolderPath)) {
										std::filesystem::path filePath = file;
										std::string extension = filePath.extension().string();

										if (extension == ".cfg") {
											if (!std::filesystem::is_directory(file.path())) {
												auto path2 = file.path().string().substr(configFolderPath.length() + 1);
												configFiles.push_back(path2.c_str());
											}
										}
									}

									static std::string current_item = configFiles.empty() ? "" : configFiles[0];

									for (int n = 0; n < configFiles.size(); n++) {
										if (configFiles[n] == "")
											continue;

										bool is_selected = (current_item == configFiles[n]);
										if (edited::Selectable(configFiles[n].c_str(), is_selected)) {
											current_item = configFiles[n];

											size_t extensionPos = current_item.find(".cfg");
											if (extensionPos != std::string::npos) {
												current_item.erase(extensionPos);
											}

											strncpy(configname, current_item.c_str(), IM_ARRAYSIZE(configname));
										}
									}





									InputTextEx("v", "Config Name", configname, 65, ImVec2(GetContentRegionMax().x - style->WindowPadding.x, 40), IM_ARRAYSIZE(configname));


									////if (edited::(XorStr("Load"))) {
									//	"chaos"::utils::configs::load(configname);
									//}



									if (edited::checkboxP("Load", &globals::useless3, ImVec2(CalcTextSize("Load").x + 5, 23))) {

										char notificationMessage[100];
										_snprintf_s(notificationMessage, sizeof(notificationMessage), "Succesfully Loaded CFG: %s", configname);

										notificationSystem.AddNotification(notificationMessage, 1000);


										chaos::utils::configs::load(configname);

										globals::useless3 = false;

									};



									ImGui::SameLine();


									if (edited::checkboxP("Save", &globals::useless2, ImVec2(CalcTextSize("save").x + 5, 23))) {

										char notificationMessage[100];
										_snprintf_s(notificationMessage, sizeof(notificationMessage), "Succesfully Saved CFG: %s", configname);

										notificationSystem.AddNotification(notificationMessage, 1000);

										chaos::utils::configs::save(configname);

										globals::useless2 = false;

									};

									ImGui::SameLine();

									if (edited::checkboxP("Delete", &globals::deletecfg, ImVec2(CalcTextSize("save").x + 5, 23))) {
										// Delete configuration logic
										char notificationMessage[100];
										_snprintf_s(notificationMessage, sizeof(notificationMessage), "Successfully Deleted CFG: %s", configname);
										notificationSystem.AddNotification(notificationMessage, 1000);
										chaos::utils::configs::deletecfg(configname); 
										globals::deletecfg = false; 
									}

									ImGui::SameLine();

									if (edited::checkboxP("Open Folder Location", &globals::useless, ImVec2(CalcTextSize("Open Folder Location").x + 5, 23))) {
										notificationSystem.AddNotification("Succusfully Opened Cfg Folder", 1000);

										globals::useless = false;

										ShellExecute(NULL, "open", "explorer.exe", GetConfigFolderPath().c_str(), NULL, SW_SHOWNORMAL);


									};
								}
								edited::EndChild();

							}
		
							else if (active_tab == 4)
							{

								edited::BeginChild("##Container0", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
								{

									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Settings");
								
									//edited::Combo("Macro Mode", "The mode of the macro", &globals::macro_mode, macro_items, IM_ARRAYSIZE(macro_items), 2);

									edited::ColorEdit4("Accented Color", "Setting the main color of the menu.", color, picker_flags);

									edited::Separator_line();

									edited::Checkbox("Smooth Camlock", "Camlock is smooth, at the expense of the cpu", &globals::smoothcamlock);
									edited::Checkbox("Esp Occlusion", "Ceases esp rendering when nearby players block the screen", &globals::nearby_check);
									
									if (!globals::game_id == 113491250) {
										edited::Checkbox("Team Check", "Ignores players on the same team", &globals::team_check);
									}

									edited::Checkbox("Health Check", "Unlocks aimbot if player health <=1 ", &globals::healthcheck);


									edited::Separator_line();


									edited::Checkbox("Hit Notifier", "Notifies hit and damage", &globals::hitnotification);

									edited::Checkbox("Masterswitch", "Enables the cheat", &globals::masterswitch);



									if (globals::hitnotification) {

										edited::Checkbox("Hit Sound", "plays sound when hit detected", &globals::hitsound);

									}

									if (globals::hitsound) {

										edited::Combo("Hit Sound", "change hit sound", &globals::hitsound_type, hit_sounds, IM_ARRAYSIZE(hit_sounds), 4);

									}


									//edited::Checkbox("Hit Notifier [Friendly]", "notifier hit n the damage.", &globals::friendlynotifier);

								//	edited::Checkbox("[Da Hood] Knock check", "Unlock aimbot if player health <= 4", &globals::knock_check);

									edited::Separator_line();


									edited::Checkbox("Streamproof", "the cheat cannot be seen by others if you screen share or record", &globals::streamproof);

									edited::Checkbox("Esp Preview", "Cooking", &enable_preview);

									//	edited::Checkbox("ESP Preview", "Activate ESP Preview for settings", &enable_preview);

									edited::Combo("Notify design", "Setting up notification styling", &notify_select, notify_items, IM_ARRAYSIZE(notify_items), 2);

								}
								edited::EndChild();
								SameLine(0, 0);


								edited::BeginChild("##Container1", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
								{
									

									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Da Hood Features");



									edited::Checkbox("Macro", "Automatically macros for you (first person)", &globals::macro);

									Keybind(&globals::macrokey, ImVec2(60, 20));


									edited::Separator_line();


									edited::Checkbox("[Da Hood] Knock check", "Unlock aimbot if player health <= 4", &globals::knock_check);
						
									edited::Separator_line();

									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Character modifications");
									edited::Checkbox("Character Modifications", "op", &globals::character_modification);

								
									edited::Checkbox("Walkspeed", "Enable walkspeed changer", &globals::walkspeed_enable);

									if (globals::character_modification && globals::walkspeed_enable) {
										edited::SliderFloat("Walkspeed", "changes walkspeed", &globals::walksspeed, 1.0, 1000.0, "%.1f");
										chaos::roblox::set_humanoid_walkspeed(globals::walksspeed);

									}

									edited::Checkbox("Jumppower", "Enables jumppower changer", &globals::jumppower_enable);


									if (globals::character_modification && globals::jumppower_enable) {
										edited::SliderFloat("Jumppower", "changes jumppower", &globals::jumppower, 1.0, 1000.0, "%.1f");
										chaos::roblox::set_humanoid_jumppower(globals::jumppower);
										
									}

									edited::Checkbox("Hipheight", "Enables Hipheight changer", &globals::hipheight_enable);

							
									if (globals::character_modification && globals::hipheight_enable) {
										edited::SliderFloat("Hipheight", "changes hipheight", &globals::hipheight, 1.0f, 1000.0f, "%.1f");
									    chaos::roblox::set_humanoid_hipheight(globals::hipheight);

									}
									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Panic Key");


									Keybind(&globals::panickey, ImVec2(60, 20));

								}

								edited::EndChild();

							}

							else if (active_tab == 5)
							{
								edited::BeginChild("##Container0", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
								{
									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Player List (AIMING)");

									std::vector<std::string> allPlayerNamesx = fetchPlayerNames();

									std::vector<std::string> playerNamesx;
									for (const auto& name : allPlayerNamesx) {
										if (globals::aimwhitelistedPlayers.find(name) == globals::aimwhitelistedPlayers.end()) {
											playerNamesx.push_back(name);
										}
									}

									static std::string current_selected_playerx;

									for (const auto& playerName : playerNamesx) {
										if (edited::Selectable(playerName.c_str(), current_selected_playerx == playerName)) {
											current_selected_playerx = playerName;
										}
									}

									static bool whitelistTrigger = false;
									if (!current_selected_playerx.empty() && edited::checkboxP("Whitelist", &whitelistTrigger, ImVec2(ImGui::CalcTextSize("Whitelist").x + 5, 23))) {
										char notificationMessage[100];
										_snprintf_s(notificationMessage, sizeof(notificationMessage), "Successfully Whitelisted Player: %s", current_selected_playerx.c_str());
										notificationSystem.AddNotification(notificationMessage, 1000);

										globals::aimwhitelistedPlayers.insert(current_selected_playerx);
										whitelistTrigger = false;

										current_selected_playerx.clear();
									}
									edited::Separator_line();




									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Whitelisted Players");
									static std::string playerToUnwhitelistx;
									static bool unwhitelistTrigger = false;
									for (const auto& name : globals::aimwhitelistedPlayers) {
										if (edited::Selectable(name.c_str(), playerToUnwhitelistx == name)) {
											playerToUnwhitelistx = name;
										}
									}

									if (!playerToUnwhitelistx.empty() && edited::checkboxP("Unwhitelist", &unwhitelistTrigger, ImVec2(ImGui::CalcTextSize("Unwhitelist").x + 5, 23))) {
										char notificationMessage[100];
										_snprintf_s(notificationMessage, sizeof(notificationMessage), "Successfully Unwhitelisted Player: %s", playerToUnwhitelistx.c_str());
										notificationSystem.AddNotification(notificationMessage, 1000);

										globals::aimwhitelistedPlayers.erase(playerToUnwhitelistx);
										unwhitelistTrigger = false;

										playerToUnwhitelistx.clear();
									}



								}
								edited::EndChild();

								SameLine(0, 0);



								edited::BeginChild("##Container1", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
								{
									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Player List (ESP)");

									std::vector<std::string> allPlayerNames = fetchPlayerNames();

									std::vector<std::string> playerNames;
									for (const auto& name : allPlayerNames) {
										if (globals::whitelistedPlayers.find(name) == globals::whitelistedPlayers.end()) {
											playerNames.push_back(name);
										}
									}

									static std::string current_selected_player;

									for (const auto& playerName : playerNames) {
										if (edited::Selectable(playerName.c_str(), current_selected_player == playerName)) {
											current_selected_player = playerName;
										}
									}

									static bool whitelistTriggerx = false;
									if (!current_selected_player.empty() && edited::checkboxP("Whitelist", &whitelistTriggerx, ImVec2(ImGui::CalcTextSize("Whitelist").x + 5, 23))) {
										char notificationMessage[100];
										_snprintf_s(notificationMessage, sizeof(notificationMessage), "Successfully Whitelisted Player: %s", current_selected_player.c_str());
										notificationSystem.AddNotification(notificationMessage, 1000);

										globals::whitelistedPlayers.insert(current_selected_player);
										whitelistTriggerx = false;

										current_selected_player.clear();
									}

									ImGui::SameLine();
									static bool buss = false;
									if (!current_selected_player.empty() && edited::checkboxP("Teleport", &buss, ImVec2(ImGui::CalcTextSize("Teleport").x + 5, 23))) {
										char notificationMessage[100];
										
										chaos::roblox::vector3_t posyes = 	globals::players.find_first_child(current_selected_player).get_model_instance().find_first_child("HumanoidRootPart").get_part_pos();


									
										globals::LocalPlayer.get_model_instance().find_first_child("HumanoidRootPart").set_part_pos(posyes);
										

										


										_snprintf_s(notificationMessage, sizeof(notificationMessage), "Successfully Teleported to  Player: %s", current_selected_player.c_str());
										notificationSystem.AddNotification(notificationMessage, 1000);
										buss = false;
										current_selected_player.clear();
									}

									ImGui::SameLine();

								
									static bool buss4 = false;
									if (!current_selected_player.empty() && edited::checkboxP("Spectate", &buss4, ImVec2(ImGui::CalcTextSize("Spectate").x + 5, 23))) {
										char notificationMessage[100];

										//	chaos::roblox::vector3_t posyes = globals::players.find_first_child(current_selected_player).get_model_instance().find_first_child("HumanoidRootPart").get_part_pos();


									
									globals::players.Spectate(current_selected_player);





										_snprintf_s(notificationMessage, sizeof(notificationMessage), "Successfully Spectated to  Player: %s", current_selected_player.c_str());
										notificationSystem.AddNotification(notificationMessage, 1000);
										buss4 = false;
										current_selected_player.clear();
									}
									
									ImGui::SameLine();

									static bool buss5 = false;
									if (!current_selected_player.empty() && edited::checkboxP("UnSpectate", &buss4, ImVec2(ImGui::CalcTextSize("UnSpectate").x + 5, 23))) {
										char notificationMessage[100];

										//	chaos::roblox::vector3_t posyes = globals::players.find_first_child(current_selected_player).get_model_instance().find_first_child("HumanoidRootPart").get_part_pos();


								
										globals::players.UnSpectate();





										_snprintf_s(notificationMessage, sizeof(notificationMessage), "Successfully UnSpectated to  Player: %s", current_selected_player.c_str());
										notificationSystem.AddNotification(notificationMessage, 1000);
										buss4 = false;
										current_selected_player.clear();
									}


									edited::Separator_line();

									ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), "Whitelisted Players");
									static std::string playerToUnwhitelist;
									static bool unwhitelistTriggerx = false;
									for (const auto& name : globals::whitelistedPlayers) {
										if (edited::Selectable(name.c_str(), playerToUnwhitelist == name)) {
											playerToUnwhitelist = name;
										}
									}

									if (!playerToUnwhitelist.empty() && edited::checkboxP("Unwhitelist", &unwhitelistTriggerx, ImVec2(ImGui::CalcTextSize("Unwhitelist").x + 5, 23))) {
										char notificationMessage[100];
										_snprintf_s(notificationMessage, sizeof(notificationMessage), "Successfully Unwhitelisted Player: %s", playerToUnwhitelist.c_str());
										notificationSystem.AddNotification(notificationMessage, 1000);

										globals::whitelistedPlayers.erase(playerToUnwhitelist);
										unwhitelistTriggerx = false;

										playerToUnwhitelist.clear(); 
									}






								}

								edited::EndChild();

								}



							ImGui::PopStyleVar();

						}
						End();
					}

					ImGui::PushFont(font::lexend_bold);


					ImGuiIO& io = ImGui::GetIO();
					float fps = io.Framerate;
					char fpsCounter[32];
					sprintf(fpsCounter, "%.1f ", fps);
					//	const char* info_set[3] = { text, fpsCounter, role };


					std::string world_time = getCurrentTime();
					//	const float world_time_size = ImGui::CalcTextSize(world_time.c_str()).x;



					const float cheat_name_size = ImGui::CalcTextSize(cheat_name).x;
					const float developer_size = ImGui::CalcTextSize(developer).x;
					const float role_size = ImGui::CalcTextSize(role).x;
					const float ping_size = ImGui::CalcTextSize(ping).x;
					const float fps_size = ImGui::CalcTextSize(fpsCounter).x;
					const float world_time_size = ImGui::CalcTextSize(world_time.c_str()).x;
					const float bar_size = ImGui::CalcTextSize("|").x;

					const float ibar_size = cheat_name_size + bar_size + developer_size + bar_size + role_size + bar_size + ping_size + fps_size + developer_size + bar_size + world_time_size * 2 + bar_size * 3;

					ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - (ibar_size + 15), 15));
					ImGui::SetNextWindowSize(ImVec2(ibar_size, 45));

					ImGui::Begin("info-bar", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
					{
						const ImVec2& pos = ImGui::GetWindowPos(), spacing = style->ItemSpacing, region = ImGui::GetContentRegionMax();

						GetBackgroundDrawList()->AddRectFilled(pos, pos + region, ImGui::GetColorU32(c::background::filling), c::elements::rounding);
						GetBackgroundDrawList()->AddRect(pos, pos + region, ImGui::GetColorU32(c::background::stroke), c::elements::rounding);

						const char* info_set[6] = { cheat_name, developer, role, ping, fpsCounter, world_time.c_str() };

						ImGui::SetCursorPos(ImVec2(spacing.x, (45 - ImGui::CalcTextSize(developer).y) / 2));
						ImGui::BeginGroup();
						{
							for (int i = 0; i < ARRAYSIZE(info_set); i++) {
								ImGui::TextColored(i < 1 ? ImColor(ImGui::GetColorU32(c::accent)) : ImColor(ImGui::GetColorU32(c::elements::text)), info_set[i]);
								ImGui::SameLine();
								ImGui::TextColored(ImColor(ImGui::GetColorU32(c::elements::text)), "|");
								ImGui::SameLine();
							}
						}
						ImGui::EndGroup();
					}
					ImGui::End();

					PopFont();


					if (enable_preview) {
						ImGui::SetNextWindowSize(ImVec2(350, 490));

						ImGui::Begin("ESP PREVIEW", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
						{
							const ImVec2& pos = ImGui::GetWindowPos(), spacing = style->ItemSpacing, region = ImGui::GetContentRegionMax();

							GetBackgroundDrawList()->AddRectFilled(pos, pos + region, ImGui::GetColorU32(c::background::filling), c::elements::rounding);
							GetBackgroundDrawList()->AddRect(pos, pos + region, ImGui::GetColorU32(c::background::stroke), c::elements::rounding);

							GetBackgroundDrawList()->AddRectFilledMultiColor(pos + ImVec2(0, region.y / 2), pos + ImVec2(2, region.y), ImGui::GetColorU32(c::accent), ImGui::GetColorU32(c::accent), ImGui::GetColorU32(c::background::filling), ImGui::GetColorU32(c::background::filling), c::elements::rounding);
							GetBackgroundDrawList()->AddRectFilledMultiColor(pos + ImVec2(0, 0), pos + ImVec2(2, region.y / 2), ImGui::GetColorU32(c::background::filling), ImGui::GetColorU32(c::background::filling), ImGui::GetColorU32(c::accent), ImGui::GetColorU32(c::accent), c::elements::rounding);

							GetBackgroundDrawList()->AddRectFilledMultiColor(pos + ImVec2(region.x - 2, region.y / 2), pos + ImVec2(region.x, region.y), ImGui::GetColorU32(c::accent), ImGui::GetColorU32(c::accent), ImGui::GetColorU32(c::background::filling), ImGui::GetColorU32(c::background::filling), c::elements::rounding);
							GetBackgroundDrawList()->AddRectFilledMultiColor(pos + ImVec2(region.x - 2, 0), pos + ImVec2(region.x, region.y / 2), ImGui::GetColorU32(c::background::filling), ImGui::GetColorU32(c::background::filling), ImGui::GetColorU32(c::accent), ImGui::GetColorU32(c::accent), c::elements::rounding);

							GetWindowDrawList()->AddText(pos + ImVec2(15, 15), GetColorU32(c::elements::text_active), "ESP PREVIEW");

							SetCursorPosY(55);
							edited::esp_preview(texture::dick,
								&esp_preview::nickname, esp_preview::nick_color,
								&esp_preview::weapon, esp_preview::weapon_color,
								&esp_preview::hp, esp_preview::hp_color,
								&esp_preview::zoom, esp_preview::zoom_color,
								&esp_preview::bomb, esp_preview::bomb_color,
								&esp_preview::c4, esp_preview::c4_color,
								&esp_preview::money, esp_preview::money_color,
								&esp_preview::hit, esp_preview::hit_color,
								&esp_preview::box, esp_preview::box_color,
								&esp_preview::HP_line, esp_preview::hp_line_color);

							ImGui::SetCursorPos(ImVec2(20, 415));
							ImGui::BeginGroup();
							{
								edited::checkboxP("Nickname", &esp_preview::nickname, ImVec2(CalcTextSize("Nickname").x + 5, 23));
								ImGui::SameLine();
								edited::checkboxP("Weapon", &esp_preview::weapon, ImVec2(CalcTextSize("Weapon").x + 5, 23));
								ImGui::SameLine();
								edited::checkboxP("Zoom", &esp_preview::zoom, ImVec2(CalcTextSize("Zoom").x + 5, 23));
								ImGui::SameLine();
								edited::checkboxP("Bomb", &esp_preview::bomb, ImVec2(CalcTextSize("Bomb").x + 5, 23));
								ImGui::SameLine();
								edited::checkboxP("HP", &esp_preview::HP_line, ImVec2(CalcTextSize("HP").x + 5, 23));

								edited::checkboxP("Money", &esp_preview::money, ImVec2(CalcTextSize("Money").x + 5, 23));
								ImGui::SameLine();
								edited::checkboxP("Hit", &esp_preview::hit, ImVec2(CalcTextSize("Hit").x + 5, 23));
								ImGui::SameLine();
								edited::checkboxP("Box", &esp_preview::box, ImVec2(CalcTextSize("Box").x + 5, 23));
								ImGui::SameLine();
								edited::checkboxP("C4", &esp_preview::c4, ImVec2(CalcTextSize("C4").x + 5, 23));

							}
							ImGui::EndGroup();

						}
						ImGui::End();
					}
				}




				ImGui::PushFont(font::lexend_regular);


				static bool notificationSent = false;

				if (globals::threadcrash == true && !notificationSent) {
					notificationSystem.AddNotification("Thread crash has detected, restarting", 1000);
					notificationSent = true;
				}
				else if (globals::threadcrash == false) {
					notificationSent = false;
				}

				static float previousHealth = -1.0f; 


				static float animationTime = 0.0f;
				static bool isAnimating = true;

				if (globals::targetmark && globals::saved_player.self != 0) {

					auto draw = ImGui::GetBackgroundDrawList();

					float animationDuration = 0.5f;
					float transparency = globals::transparent ? 128 : 255;

					float x = 8.5;
					float y = 80;

					if (globals::lowerbottom) {
						x = (ImGui::GetIO().DisplaySize.x - 207) / 2;
						y = ImGui::GetIO().DisplaySize.y - 80 - (60 / 2);
					}

					if (isAnimating) {
						transparency *= (animationTime / animationDuration);
					}
					else {
						transparency *= ((animationDuration - fmin(animationTime, animationDuration)) / animationDuration);
					}

					float initialWidth = 207;
					float additionalWidth = 0;
					float watermarkHeight = 60;
					float textOffsetY = 10;

					std::string main = "";

					auto character = globals::saved_player.get_model_instance();
					auto humanoid = character.find_first_child("Humanoid");

					auto humanoidrootpart = character.find_first_child("HumanoidRootPart");
					auto hrp_pos = humanoidrootpart.get_part_pos();


					if (humanoid.self) {
						auto targetName = globals::saved_player.name();
						float health = humanoid.get_health();
						float currentHealth = humanoid.get_health();

						chaos::roblox::instance_t armor_path = character.find_first_child("BodyEffects").find_first_child("Armor");

						float armor = armor_path.get_armor();


						int healthPercentage = static_cast<int>(health);

						int ArmorPercentage = static_cast<int>(armor);


						if (previousHealth >= 0.0f && currentHealth < previousHealth && currentHealth > 0) {
						
							float damageDealt = previousHealth - currentHealth;
							std::string hitNotification = "Hit detected on " + targetName + "! Damage: " + std::to_string(static_cast<int>(damageDealt)) + " HP.";
							notificationSystem.AddNotification(hitNotification, 1000);

						}

						previousHealth = currentHealth;


						std::string healthStatus = "Healthy";

						if (healthPercentage <= 0)
							healthStatus = "Dead";
						else if (healthPercentage <= 25)
							healthStatus = "Critical";
						else if (healthPercentage <= 50)
							healthStatus = "Wounded";
						else if (healthPercentage <= 75)
							healthStatus = "Hurt";
						auto camerapos = globals::visualengine.get_camera_pos();


				
						auto distance_from_camera = vector_ops_x::magnitude(vector_ops_x::subtract(camerapos, hrp_pos));

						std::string distanceText = std::to_string(static_cast<int>(distance_from_camera)) + "m";

						std::string targetInfo = "\nTarget Name: " + targetName + "\nHealth: " + std::to_string(healthPercentage) + " HP" + "\nArmor: " + std::to_string(ArmorPercentage);

						main = targetInfo;

						additionalWidth = 40;
					}
					else {
						isAnimating = true;
						previousHealth = -1.0f;

					}

					ImVec2 watermarkPos(x, y);
					ImVec2 watermarkSize(initialWidth + additionalWidth, watermarkHeight);

					draw->AddRectFilled(watermarkPos, ImVec2(watermarkPos.x + watermarkSize.x, watermarkPos.y + watermarkSize.y), IM_COL32(28, 28, 28, static_cast<int>(transparency)));
					draw->AddRectFilled(ImVec2(watermarkPos.x + 1, watermarkPos.y + 1), ImVec2(watermarkPos.x + watermarkSize.x - 1, watermarkPos.y + watermarkSize.y - 1), IM_COL32(47, 48, 46, static_cast<int>(transparency)));
					draw->AddRectFilled(ImVec2(watermarkPos.x + 3, watermarkPos.y + 3), ImVec2(watermarkPos.x + watermarkSize.x - 3, watermarkPos.y + watermarkSize.y - 3), IM_COL32(21, 24, 21, static_cast<int>(transparency)));
					draw->AddRectFilled(ImVec2(watermarkPos.x + 3.3f, watermarkPos.y + 3.3f), ImVec2(watermarkPos.x + watermarkSize.x - 3.3f, watermarkPos.y + 4.3f), IM_COL32_WHITE);

					draw->AddText(ImVec2(watermarkPos.x + 5, watermarkPos.y + 5 - textOffsetY), IM_COL32_WHITE, main.c_str());

					if (isAnimating) {
						animationTime += ImGui::GetIO().DeltaTime;
						if (animationTime >= animationDuration) {
							animationTime = 0.0f;
							isAnimating = false;
						}
					}
				}
				else {
					animationTime = 0.0f;
					previousHealth = -1.0f;

					isAnimating = true;
				}


				static float previousHealthx = -1.0f;
				//PopFont();

					//PushFont(font::monospace);

				if (globals::hitnotification && globals::saved_player.self != 0) {


					//PopFont();

					//PushFont(font::monospace);

					auto character = globals::saved_player.get_model_instance();
					auto humanoid = character.find_first_child("Humanoid");

					auto hrp_pos = humanoid.get_part_pos();


					if (humanoid.self) {
						auto targetName = globals::saved_player.name();
						float health = humanoid.get_health();
						float currentHealth = humanoid.get_health();

						int healthPercentage = static_cast<int>(health);


						if (previousHealthx >= 0.0f && currentHealth < previousHealthx && currentHealth > 0) {

							float damageDealt = previousHealthx - currentHealth;
							std::string hitNotification = "Hit detected on " + targetName + "! Damage: " + std::to_string(static_cast<int>(damageDealt)) + " HP.";
							chaos::roblox::AddNotification(hitNotification, 1.0f);
						//	notificationSystem.AddNotification(hitNotification, 1000);
							if (globals::hitsound) {

								if (globals::hitsound_type == 0) {

									PlaySoundA(reinterpret_cast<char*>(neverlose_sound), NULL, SND_ASYNC | SND_MEMORY);

								}
								else if (globals::hitsound_type == 1) {
									PlaySoundA(reinterpret_cast<char*>(skeet_sound), NULL, SND_ASYNC | SND_MEMORY);

								}
								else if (globals::hitsound_type == 2) {
									PlaySoundA(reinterpret_cast<char*>(Fuck), NULL, SND_ASYNC | SND_MEMORY);

								}
								else if (globals::hitsound_type == 3) {
									PlaySoundA(reinterpret_cast<char*>(Senpai), NULL, SND_ASYNC | SND_MEMORY);

								}

							}

						}

						previousHealthx = currentHealth;



					}
					else {
						previousHealthx = -1.0f;

					}
				}







		//		CheckHealthAndNotify();
		
			


				PopFont();


				ImGui::PushFont(font::lexend_general_bold);

				if (globals::streamproof)
				{
					SetWindowDisplayAffinity(hw, WDA_EXCLUDEFROMCAPTURE);
				}
				else
				{
					SetWindowDisplayAffinity(hw, WDA_NONE);
				}

				//if (globals::show_auth)
				if (draw)
				{
					SetWindowLong(hw, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
				}
				else
				{
					SetWindowLong(hw, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
				}

				ImGui::EndFrame();
				ImGui::Render();

				const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
				d3d11_device_context->OMSetRenderTargets(1, &d3d11_render_target_view, nullptr);
				d3d11_device_context->ClearRenderTargetView(d3d11_render_target_view, clear_color_with_alpha);
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				if (globals::vsync)
				{
					dxgi_swap_chain->Present(1, 0);
				}
				else
				{
					dxgi_swap_chain->Present(0, 0);

				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}

	init = false;

	


	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	cleanup_device_d3d();
	DestroyWindow(hw);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}


void chaos::utils::overlay::move_window(HWND hw)
{
	HWND target = globals::window_handle;
	HWND foregroundWindow = GetForegroundWindow();

	if (target != foregroundWindow && hw != foregroundWindow)
	{
		MoveWindow(hw, 0, 0, 0, 0, true);
	}
	else
	{
		RECT rect;
		GetWindowRect(target, &rect);

		int rsize_x = rect.right - rect.left;
		int rsize_y = rect.bottom - rect.top;

		if (fullsc(target))
		{
			rsize_x += 16;
			rsize_y -= 24;
		}
		else
		{
			rsize_y -= 63;
			rect.left += 8;
			rect.top += 31;
		}

		MoveWindow(hw, rect.left, rect.top, rsize_x, rsize_y, TRUE);
	}
}

bool chaos::utils::overlay::create_device_d3d(HWND hw)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 75;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hw;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	const UINT create_device_flags = 0;
	D3D_FEATURE_LEVEL d3d_feature_level;
	const D3D_FEATURE_LEVEL feature_level_arr[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_device_flags, feature_level_arr, 2, D3D11_SDK_VERSION, &sd, &dxgi_swap_chain, &d3d11_device, &d3d_feature_level, &d3d11_device_context);

	if (res == DXGI_ERROR_UNSUPPORTED)
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, create_device_flags, feature_level_arr, 2, D3D11_SDK_VERSION, &sd, &dxgi_swap_chain, &d3d11_device, &d3d_feature_level, &d3d11_device_context);
	if (res != S_OK)
		return false;

	create_render_target();
	return true;
}

void chaos::utils::overlay::cleanup_device_d3d()
{
	cleanup_render_target();

	if (dxgi_swap_chain)
	{
		dxgi_swap_chain->Release();
		dxgi_swap_chain = nullptr;
	}

	if (d3d11_device_context)
	{
		d3d11_device_context->Release();
		d3d11_device_context = nullptr;
	}

	if (d3d11_device)
	{
		d3d11_device->Release();
		d3d11_device = nullptr;
	}
}

void chaos::utils::overlay::create_render_target()
{
	ID3D11Texture2D* d3d11_back_buffer;
	dxgi_swap_chain->GetBuffer(0, IID_PPV_ARGS(&d3d11_back_buffer));
	if (d3d11_back_buffer != nullptr)
	{
		d3d11_device->CreateRenderTargetView(d3d11_back_buffer, nullptr, &d3d11_render_target_view);
		d3d11_back_buffer->Release();
	}
}

void chaos::utils::overlay::cleanup_render_target()
{
	if (d3d11_render_target_view)
	{
		d3d11_render_target_view->Release();
		d3d11_render_target_view = nullptr;
	}
}

LRESULT __stdcall chaos::utils::overlay::window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (d3d11_device != nullptr && wParam != SIZE_MINIMIZED)
		{
			cleanup_render_target();
			dxgi_swap_chain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			create_render_target();
		}
		return 0;

	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}