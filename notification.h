#pragma once

#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_toggle.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include <windows.h>
#include <chrono>
#include <ctime>

#include <imgui.h>

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

		ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(pos, pos + region, ImGui::GetColorU32(c::background::filling, alpha), ImGui::GetColorU32(c::accent, 0.01f), ImGui::GetColorU32(c::accent, 0.01f), ImGui::GetColorU32(c::background::filling, alpha), c::elements::rounding);
		ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + region, ImGui::GetColorU32(c::background::filling, 0.4f), c::elements::rounding);
		ImGui::GetBackgroundDrawList()->AddRect(pos, pos + region, ImGui::GetColorU32(c::background::stroke, alpha), c::elements::rounding);

		if (ImGui::notify_select == 0)
			ImGui::CricleProgress("##NOTIFY", percentage, 100, 7.f, ImVec2(GetContentRegionMax().x - 40, 11));
		if (notify_select == 1)
			ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, region.y - 3), pos + ImVec2(region.x * (duePercentage / 100.0f), region.y), ImGui::GetColorU32(c::accent, alpha), c::elements::rounding);

		ImGui::PushFont(font::lexend_bold);
		ImGui::TextColored(ImColor(ImGui::GetColorU32(c::accent, alpha)), "%s", "[d3p0s1t]");
		ImGui::TextColored(ImColor(ImGui::GetColorU32(c::elements::text_active, alpha)), "%s", message.c_str());
		ImGui::Dummy(ImVec2(ImGui::CalcTextSize(message.c_str()).x + 15, 5));
		ImGui::PopFont();

		ImGui::End();
	}
};

NotificationSystem notificationSystem;