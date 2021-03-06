#include <switch.h>

#include "common.h"
#include "config.h"
#include "dirbrowse.h"
#include "menu_main.h"
#include "menu_options.h"
#include "SDL_helper.h"
#include "status_bar.h"

static void Menu_HomeControls(u64 input)
{
	if (input & KEY_PLUS)
		longjmp(exitJmp, 1);

	if (fileCount > 0)
	{
		if (input & KEY_DUP)
		{
			// Decrease Position
			if (position > 0)
				position--;

			// Rewind Pointer
			else 
				position = fileCount - 1;
		}
		else if (input & KEY_DDOWN)
		{
			// Increase Position
			if (position < (fileCount - 1))
				position++;

			// Rewind Pointer
			else 
				position = 0;
		}

		if (input & KEY_X)
		{
			if (MENU_DEFAULT_STATE == MENU_STATE_OPTIONS)
				MENU_DEFAULT_STATE = MENU_STATE_HOME;
			else
				MENU_DEFAULT_STATE = MENU_STATE_OPTIONS;
		}

		if (input & KEY_A)
		{
			wait(5);
			Dirbrowse_OpenFile();
		}

		if ((strcmp(cwd, ROOT_PATH) != 0) && (input & KEY_B))
		{
			wait(5);
			Dirbrowse_Navigate(-1);
			Dirbrowse_PopulateFiles(true);
		}
	}
}

static void Menu_Main_Controls(void)
{
	u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
	
	if (MENU_DEFAULT_STATE == MENU_STATE_HOME)
		Menu_HomeControls(kDown);
	else if (MENU_DEFAULT_STATE == MENU_STATE_OPTIONS)
		Menu_ControlOptions(kDown);
	else if (MENU_DEFAULT_STATE == MENU_STATE_PROPERTIES)
		Menu_ControlProperties(kDown);
	else if (MENU_DEFAULT_STATE == MENU_STATE_DIALOG)
		Menu_ControlDeleteDialog(kDown);
}

void Menu_Main(void)
{
	Dirbrowse_PopulateFiles(false);

	while(appletMainLoop())
	{
		SDL_ClearScreen(RENDERER, dark_theme? BLACK_BG : WHITE);
		SDL_RenderClear(RENDERER);
		SDL_DrawRect(RENDERER, 0, 0, 1280, 40, dark_theme? STATUS_BAR_DARK : STATUS_BAR_LIGHT);	// Status bar
		SDL_DrawRect(RENDERER, 0, 40, 1280, 100, dark_theme? MENU_BAR_DARK : MENU_BAR_LIGHT);	// Menu bar
		
		StatusBar_DisplayTime();
		Dirbrowse_DisplayFiles();

		hidScanInput();
		Menu_Main_Controls();

		if (MENU_DEFAULT_STATE == MENU_STATE_OPTIONS)
			Menu_DisplayOptions();
		else if (MENU_DEFAULT_STATE == MENU_STATE_PROPERTIES)
			Menu_DisplayProperties();
		else if (MENU_DEFAULT_STATE == MENU_STATE_DIALOG)
			Menu_DisplayDeleteDialog();
		
		SDL_RenderPresent(RENDERER);
	}
}