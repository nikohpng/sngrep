/**************************************************************************
 **
 ** sngrep - SIP Messages flow viewer
 **
 ** Copyright (C) 2013-2016 Ivan Alonso (Kaian)
 ** Copyright (C) 2013-2016 Irontec SL. All rights reserved.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/
/**
 * @file ui_manager.h
 * @author Ivan Alonso [aka Kaian] <kaian@irontec.com>
 *
 * @brief Functions to manage interface panels
 *
 * All sngrep panel pointers are encapsulated into a ui structure that is
 * used to invoke custom functions for creating, destroying, drawing, etc
 * the screens.
 *
 * This sctructure also manages concurrents updates and access to ncurses
 * panel pointers.
 *
 */
#ifndef __SNGREP_UI_MANAGER_H
#define __SNGREP_UI_MANAGER_H
#include "config.h"
#include "ui_panel.h"
#include "sip.h"
#include "group.h"
#include "keybinding.h"
#include "setting.h"

//! Refresh UI every 200 ms
#define REFRESHTHSECS   2
//! Default dialog dimensions
#define DIALOG_MAX_WIDTH 100
#define DIALOG_MIN_WIDTH 40

/**
 * @brief Enum for available color pairs
 */
enum sngrep_colors_pairs {
    CP_DEFAULT = 0,
    CP_CYAN_ON_DEF,
    CP_YELLOW_ON_DEF,
    CP_MAGENTA_ON_DEF,
    CP_GREEN_ON_DEF,
    CP_RED_ON_DEF,
    CP_BLUE_ON_DEF,
    CP_WHITE_ON_DEF,
    CP_DEF_ON_CYAN,
    CP_DEF_ON_BLUE,
    CP_WHITE_ON_BLUE,
    CP_BLACK_ON_CYAN,
    CP_WHITE_ON_CYAN,
    CP_YELLOW_ON_CYAN,
    CP_BLUE_ON_CYAN,
    CP_BLUE_ON_WHITE,
    CP_CYAN_ON_BLACK,
    CP_CYAN_ON_WHITE,
};

// Used to configure color pairs only with fg color
#define COLOR_DEFAULT -1

/**
 * Define existing panels
 */
extern ui_panel_t ui_call_list;
extern ui_panel_t ui_call_flow;
extern ui_panel_t ui_call_raw;
extern ui_panel_t ui_filter;
extern ui_panel_t ui_save;
extern ui_panel_t ui_msg_diff;
extern ui_panel_t ui_column_select;
extern ui_panel_t ui_settings;
extern ui_panel_t ui_stats;

/**
 * @brief Initialize ncurses mode
 *
 * This functions will initialize ncurses mode
 *
 * @returns 0 on ncurses initialization success, 1 otherwise
 */
int
ncurses_init();

/**
 * @brief Stops ncurses mode
 *
 * This functions will deinitialize ncurse mode
 *
 * @returns 0 on ncurses initialization success, 1 otherwise
 */
void
ncurses_deinit();


/**
 * @brief Create a panel of a given type
 *
 * Create a ncurses panel of the given type.
 * This function is a small wrapper for panel create function
 *
 * @param type Panel Type
 * @return the ui structure with the panel pointer created*
 */
ui_panel_t *
ui_create_panel(enum panel_types type);

/**
 * @brief Find a ui from its pannel pointer
 */
ui_panel_t *
ui_find_by_panel(PANEL *panel);

/**
 * @brief Find a ui form its panel id
 */
ui_panel_t *
ui_find_by_type(enum panel_types type);

/**
 * @brief Wait for user input in topmost panel
 *
 * This function manages all user input in all panel types and
 * redraws the panel using its own draw function
 *
 */
int
wait_for_input();

/**
 * @brief Default handler for keys
 *
 * If ui doesn't handle the given key (ui_handle_key returns -1)
 * then the default handler will be invoked
 *
 * @param ui Current displayed UI structure
 * @param key key pressed by user
 */
int
default_handle_key(ui_panel_t *ui, int key);

/**
 * @brief Draw a box around passed windows
 *
 * Draw a box around passed windows  with two bars
 * (top and bottom) of one line each.
 *
 * @param win Window to draw borders on
 */
void
title_foot_box(PANEL *panel);

/**
 * @brief Draw title at the top of the panel
 *
 * This function will draw a line with the title on the first
 * row of the panel's window
 *
 * @param panel PANEL pointer to draw title on
 * @param title String containing the title
 */
void
draw_title(PANEL *panel, const char *title);

/**
 * @brief Draw keybinding info at the bottom of the panel
 *
 * This function will draw a line with the available keybindings
 * in the last line of the given panel
 *
 */
void
draw_keybindings(PANEL *panel, const char *keybindings[], int count);

/**
 * @brief Draw a vertical scroll
 *
 * This function will draw a vertical scroll in the
 * right or left side of the given window
 */
void
draw_vscrollbar(WINDOW *win, int value, int max, int left);

/**
 * @brief Clear a given window line
 *
 * This function can be used to clear a given line on the
 * screen.
 */
void
clear_line(WINDOW *win, int line);

/**
 * @brief Draw a message payload in a window
 *
 * Generic drawing function for payload. This function will start
 * writting at 0,0 and return the number of lines written.
 *
 * @param win Ncurses window to draw payload
 * @param msg Msg to be drawn
 */
int
draw_message(WINDOW *win, sip_msg_t *msg);

/**
 * @brief Draw a message payload in a window starting at a given line
 *
 * Generic drawing function for payload. This function will start
 * writting at line starting and first column and return the number
 * of lines written.
 *
 * @param win Ncurses window to draw payload
 * @param msg Msg to be drawn
 * @param starting Number of win line to start writting payload
 */
int
draw_message_pos(WINDOW *win, sip_msg_t *msg, int starting);

/**
 * @brief Draw a centered dialog with a message
 *
 * Create a centered dialog with a message.
 * @param msg Message to be drawn
 */
int
dialog_run(const char *fmt, ...);

/**
 * @brief Create a new progress bar dialog
 *
 * Create a new progress bar dialog with the given text. The returned
 * pointer should be used as parameter for @dialog_progress_set_value
 * in order to move the progress bar percentage.
 *
 * @param fmt, vaarg Text to be displayed above the progress bar
 * @return a pointer to the created window.
 */
WINDOW *
dialog_progress_run(const char *fmt, ...);

/**
 * @brief Set current percentage of dialog progress bar
 *
 * @param win Window pointer created with @dialog_progress_run
 * @param perc 0-100 percentage of progress bar
 */
void
dialog_progress_set_value(WINDOW *win, int perc);

/**
 * @brief Destroy a dialog created by @dialog_progress_run
 *
 * This function will deallocate all memory and close the
 * given window pointer.
 *
 * @param win Window pointer created with @dialog_progress_run
 */
void
dialog_progress_destroy(WINDOW *win);

/**
 * @brief Create a new confirmation dialog with multiple buttons
 *
 * This function can be used to create dialogs with multiple buttons to
 * request user confirmation. By default, the first given option will
 * be selected.
 *
 * @param title Title displayed in the top of the dialog
 * @param text Text displayed inside the dialog
 * @param options Comma separated labels for the different buttons
 * @return the index of the button pressed
 */
int
dialog_confirm(const char *title, const char *text, const char *options);

#endif    // __SNGREP_UI_MANAGER_H
