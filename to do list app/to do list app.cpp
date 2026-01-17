#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#include "math.h"
#include <stdio.h>
#include <string.h>

int main()
{
	Color textcolor = { 200,180,190,255 };
	InitWindow(600, 800, "To do App");
	SetTargetFPS(60);
	Texture2D logo = LoadTexture("logo.png");
	int containerWidth = 500;
	int containerHeight = 700;
	int containerX = (600 - containerWidth) / 2;
	int containerY = (800 - containerHeight) / 2;
	int logoX = containerX + (containerWidth - logo.width * 0.5f) / 2;
	int logoY = containerY + 50;
	float roundness = 0.1f;
	Font customFont = LoadFont("font.ttf");
	const char* titleText = "Turn Chaos into Checkmarks";
	int fontSize = 36;
	int textY = logoY + (logo.height * 0.5f) - 70;
	int taskContainerWidth = 350;
	int taskContainerHeight = 270;
	int taskContainerX = containerX + (containerWidth - taskContainerWidth) / 2;
	int taskContainerY = textY + 60;
	int rowHeight = 40;
	int headerY = taskContainerY + 15;
	int rowStartY = headerY + 45;
	int starSize = 16;
	int starX = taskContainerX + 25;
	int taskTextX = starX + starSize + 25;

	bool starStates[5] = { false, false, false, false, false };
	char taskTexts[5][100] = { "", "", "", "", "" };
	int activeTextBox = -1;
	int finishButtonWidth = 150;
	int finishButtonHeight = 40;
	int finishButtonX = containerX + (containerWidth - finishButtonWidth) / 2;
	int finishButtonY = taskContainerY + taskContainerHeight + 20;
	bool showMessage = false;
	char resultMessage[50] = "";

	while (!WindowShouldClose())
	{
		Vector2 mousePos = GetMousePosition();
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			for (int i = 0; i < 5; i++)
			{
				int clickRowY = rowStartY + (i * rowHeight);
				if (mousePos.x >= starX && mousePos.x <= starX + starSize &&
					mousePos.y >= clickRowY && mousePos.y <= clickRowY + rowHeight)
				{
					starStates[i] = !starStates[i];
					printf("Star %d clicked! State: %s\n", i + 1, starStates[i] ? "filled" : "empty");
				}
			}
			activeTextBox = -1;
			for (int i = 0; i < 5; i++) {
				int clickRowY = rowStartY + (i * rowHeight);
				int taskTextWidth = taskContainerWidth - (starSize + 60);
				if (mousePos.x >= taskTextX && mousePos.x <= taskTextX + taskTextWidth &&
					mousePos.y >= clickRowY && mousePos.y <= clickRowY + rowHeight) {
					activeTextBox = i;
					printf("Text box %d activated\n", i + 1);
				}
			}
		}
		if (activeTextBox >= 0)
		{
			int key = GetCharPressed();
			while (key > 0) {
				if (key >= 32 && key <= 125 && strlen(taskTexts[activeTextBox]) < 99)
				{
					int len = strlen(taskTexts[activeTextBox]);
					taskTexts[activeTextBox][len] = (char)key;
					taskTexts[activeTextBox][len + 1] = '\0';
					printf("Added character: %c to task %d\n", (char)key, activeTextBox + 1);
				}
				key = GetCharPressed();
			}
			if (IsKeyPressed(KEY_BACKSPACE))
			{
				int len = strlen(taskTexts[activeTextBox]);
				if (len > 0)
				{
					taskTexts[activeTextBox][len - 1] = '\0';
					printf("Backspace in task %d\n", activeTextBox + 1);
				}
			}

		}
		if (mousePos.x >= finishButtonX && mousePos.x <= finishButtonX + finishButtonWidth &&
			mousePos.y >= finishButtonY && mousePos.y <= finishButtonY + finishButtonHeight)
		{
			if (!showMessage)
			{
				int filledCount = 0;
				for (int j = 0; j < 5; j++)
				{
					if (starStates[j]) filledCount++;
				}
				switch (filledCount)
				{
				case 5: strcpy(resultMessage, "Perfect Shining Star!"); break;
				case 4: strcpy(resultMessage, "Keep crushing it."); break;
				case 3: strcpy(resultMessage, "A hat-trick"); break;
				case 2: strcpy(resultMessage, "Only 2? you ok?"); break;
				case 1: strcpy(resultMessage, "All day and one task?"); break;
				case 0: strcpy(resultMessage, "Day wasted."); break;
				}
				showMessage = true;
				printf("Finish clicked! %d stars filled. Message: %s\n", filledCount, resultMessage);
			}
		}
		BeginDrawing();
		ClearBackground(BLACK);
		Color appColor = { 13, 9, 42, 255 };
		Rectangle appRect = { containerX, containerY, containerWidth, containerHeight };
		DrawRectangleRounded(appRect, roundness, 10, appColor);
		Vector2 logoPos = { logoX, logoY };
		DrawTextureEx(logo, logoPos, 0.0f, 0.5f, textcolor);
		Vector2 textSize = MeasureTextEx(customFont, titleText, fontSize, 1);
		int textX = containerX + (containerWidth - textSize.x) / 2;
		Vector2 textPos = { textX, textY };
		DrawTextEx(customFont, titleText, textPos, fontSize, 1, textcolor);
		Rectangle taskRect = { taskContainerX, taskContainerY, taskContainerWidth, taskContainerHeight };
		DrawRectangleRoundedLines(taskRect, 0.08f, 10, textcolor);

		const char* headerText = "your main 5 tasks";
		int headerFontSize = 36;
		Vector2 headerSize = MeasureTextEx(customFont, headerText, headerFontSize, 1);
		int headerX = taskContainerX + (taskContainerWidth - headerSize.x) / 2;
		Vector2 headerPos = { headerX, headerY };
		DrawTextEx(customFont, headerText, headerPos, headerFontSize, 1, textcolor);
		DrawLine(taskContainerX, headerY + 35, taskContainerX + taskContainerWidth, headerY + 35, textcolor);
		for (int i = 0; i < 5; i++) {
			int rowY = rowStartY + (i * rowHeight);
			if (i < 4) {
				DrawLine(taskContainerX, rowY + rowHeight - 5, taskContainerX + taskContainerWidth, rowY + rowHeight - 5, textcolor);
			}
			Vector2 starCenter = { starX + starSize / 2, rowY + rowHeight / 2 };
			float outerRadius = starSize / 2;
			float innerRadius = outerRadius * 0.4f;

			if (starStates[i]) {
				for (int j = 0; j < 5; j++) {
					float angle1 = (j * 72.0f - 90.0f) * PI / 180.0f;
					float angle2 = ((j + 1) * 72.0f - 90.0f) * PI / 180.0f;
					float angleInner = (j * 72.0f + 36.0f - 90.0f) * PI / 180.0f;

					Vector2 outer1 = { starCenter.x + cos(angle1) * outerRadius, starCenter.y + sin(angle1) * outerRadius };
					Vector2 outer2 = { starCenter.x + cos(angle2) * outerRadius, starCenter.y + sin(angle2) * outerRadius };
					Vector2 inner = { starCenter.x + cos(angleInner) * innerRadius, starCenter.y + sin(angleInner) * innerRadius };

					if (starStates[i]) {

						Vector2 starPoints[10];
						for (int j = 0; j < 10; j++) {
							float angle = (j * 36.0f - 90.0f) * PI / 180.0f;
							float radius = (j % 2 == 0) ? outerRadius : innerRadius;
							starPoints[j].x = starCenter.x + cos(angle) * radius;
							starPoints[j].y = starCenter.y + sin(angle) * radius;
						}
						DrawTriangleFan(starPoints, 10, textcolor);
					}
				}
			}
			else {
				Vector2 starPoints[10];
				for (int j = 0; j < 10; j++) {
					float angle = (j * 36.0f - 90.0f) * PI / 180.0f;
					float radius = (j % 2 == 0) ? outerRadius : innerRadius;
					starPoints[j].x = starCenter.x + cos(angle) * radius;
					starPoints[j].y = starCenter.y + sin(angle) * radius;
				}

				for (int j = 0; j < 10; j++) {
					int next = (j + 1) % 10;
					DrawLineV(starPoints[j], starPoints[next], LIGHTGRAY);
				}
			}
			Vector2 textInputPos = { taskTextX, rowY + 8 };

			if (strlen(taskTexts[i]) > 0) {
				DrawTextEx(customFont, taskTexts[i], textInputPos, 36, 1, textcolor);
			}
			else if (activeTextBox == i) {
				DrawTextEx(customFont, "_", textInputPos, 16, 1, LIGHTGRAY);
			}
			if (activeTextBox == i) {
				int taskTextWidth = taskContainerWidth - (starSize + 60);
				DrawRectangleLines(taskTextX - 5, rowY + 2, taskTextWidth, rowHeight - 4, textcolor);
			}
		}

		Rectangle finishRect = { finishButtonX, finishButtonY, finishButtonWidth, finishButtonHeight };
		DrawRectangleRounded(finishRect, 0.1f, 10, textcolor);

		const char* finishText = "Finish";
		Vector2 finishTextSize = MeasureTextEx(customFont, finishText, 24, 1);
		int finishTextX = finishButtonX + (finishButtonWidth - finishTextSize.x) / 2;
		int finishTextY = finishButtonY + (finishButtonHeight - finishTextSize.y) / 2;
		Vector2 finishTextPos = { finishTextX, finishTextY };
		DrawTextEx(customFont, finishText, finishTextPos, 24, 1, appColor);

		if (showMessage)
		{
			Vector2 messageSize = MeasureTextEx(customFont, resultMessage, 28, 1);
			int messageX = containerX + (containerWidth - messageSize.x) / 2;
			int messageY = finishButtonY + 60;
			Vector2 messagePos = { messageX, messageY };
			DrawTextEx(customFont, resultMessage, messagePos, 28, 1, textcolor);
		}

		EndDrawing();
	}

	UnloadTexture(logo);
	UnloadFont(customFont);
	CloseWindow();
	return 0;
}