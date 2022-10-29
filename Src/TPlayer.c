/*!
@file	  TPlayer.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     26/10/2022
@brief    This source file
________________________________________________________________________________________________________*/

#include "ColorTable.h"

#include "UIManager.h" // For click detection

#include "TManager.h" 
#include "TPlayer.h" 
#include "GameLoop.h"

PlayerHandSlot hand[HAND_SIZE];
PlayerHandSlot peek_hand[PEEK_SIZE];

CP_Vector text_peek_pos;

_Bool is_piece_held; // Flag for when the player clicks and holds on a Tetris Piece
PlayerHandSlot *piece_held; // The pointer to the piece held, if any

#pragma region
void RecalculateHandRenderPositions(void);
void PlayPiece(int played_index);
#pragma endregion Forward Declarations

//______________________________________________________________
// All "public" functions (Basically those in the TPlayer.h)

/*______________________________________________________________
@brief Needs to be called at the start of the game level (and after the Tetris Pieces
       has been initialized) so that the player has pieces to play with.
*/
void TPlayerInit(void) {
    //______________________________________________________________
    // Fill the player's hand and peek queue
    // First, fill the player's hand
    PlayerHandSlot *current;
    for (int index = 0; index < HAND_SIZE; ++index) {
        current = &hand[index];
        current->piece = DrawFromBag();
    }

    // Second, fill the peek's queue (Basically the upcoming pieces)
    for (int index = 0; index < PEEK_SIZE; ++index) {
        current = &peek_hand[index];
        current->piece = DrawFromBag();
    }

    // Update the positions to draw all the slots
    RecalculateHandRenderPositions();

    // Subscribe player input to player turn update
    Subscribe_PlayerTurnUpdate(TPlayerProcessInput);

    // Reset the piece held to nothing, just in case
    piece_held = NULL;
}

void TPlayerProcessInput(void) {
    // When the player first clicks
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1)) {
        CP_Vector mouse_pos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
        PlayerHandSlot *current;

        // Check which slot the player which on, if any
        for (int index = 0; index < HAND_SIZE; ++index) {
            current = &hand[index];
            if (MouseWithinArea(current->pos.x, current->pos.y, hand_slot_length, hand_slot_length, mouse_pos.x, mouse_pos.y, CP_POSITION_CORNER)) {
                // We found the slot clicked! Set the flags to true
                is_piece_held = TRUE;
                piece_held = current;

                // TODO: LINK WITH ACTUAL GRID SETTINGS
                // Change the render color & size to match the grid
                piece_held->piece.color = MENU_RED;
                piece_held->piece.color_stroke = TRANSPERANT;
                piece_held->piece.x_screen_length = 75.0f;
                piece_held->piece.y_screen_length = 75.0f;
                break;
            }
        }
    }

    // While the player is holding a piece
    if (is_piece_held) {
        CP_Vector mouse_pos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
        piece_held->piece.draw_pos.x = mouse_pos.x;
        piece_held->piece.draw_pos.y = mouse_pos.y;
    }
    
    // When the player lets go of a piece
    if (CP_Input_MouseReleased(MOUSE_BUTTON_1) && is_piece_held) {
        if (0) {
            // JARRETT TODO: If piece is dropped onto the grid, get the corresponding pos and do something
            // PlayPiece();
        }
        else {
            // If piece is dropped outside the grid, return it and carry on
            piece_held->piece.draw_pos.x = piece_held->pos.x + (SHAPE_BOUNDS - piece_held->piece.x_length) / 2.0f * hand_tile_length;
            piece_held->piece.draw_pos.y = piece_held->pos.y + (SHAPE_BOUNDS - piece_held->piece.y_length) / 2.0f * hand_tile_length;
        }

        // Reset the render color & size to match the grid
        piece_held->piece.color = WHITE;
        piece_held->piece.color_stroke = BLACK;
        piece_held->piece.x_screen_length = hand_tile_length;
        piece_held->piece.y_screen_length = hand_tile_length;

        // Remove the flags for piece_held
        is_piece_held = FALSE;
        piece_held = NULL;
    }
}

/*______________________________________________________________
@brief Renders the panel at the bottom of the panel, consisting of the player's hand, peek queue, and
       supporting text like "NEXT"
*/
void RenderHand(void) {
    PlayerHandSlot *current;

    //______________________________________________________________
    // Render each piece in the player's hand
    for (int index = 0; index < HAND_SIZE; ++index) {
        current = &hand[index];

        CP_Settings_StrokeWeight(hand_tile_stroke);

        // Render the background square surrounding each piece
        CP_Settings_Fill(MENU_BLACK);
        CP_Settings_Stroke(current->piece.color);
        CP_Graphics_DrawRect(current->pos.x, current->pos.y, hand_slot_length, hand_slot_length);

        // Settings for tile rendering
        CP_Settings_Fill(current->piece.color);
        CP_Settings_Stroke(current->piece.color_stroke);
        // Render each tile in the Tetris Piece
        float *x_screen_length = &current->piece.x_screen_length;
        float *y_screen_length = &current->piece.y_screen_length;
        for (int index_x = 0; index_x < SHAPE_BOUNDS; ++index_x) {
            for (int index_y = 0; index_y < SHAPE_BOUNDS; ++index_y) {
                if (current->piece.shape[index_x][index_y]) {
                    CP_Graphics_DrawRect(current->piece.draw_pos.x + index_x * *x_screen_length, current->piece.draw_pos.y + index_y * *y_screen_length, *x_screen_length, *y_screen_length);
                }
            }
        }
    }

    //______________________________________________________________
    // Render each piece in the peek queue & the "NEXT" text
    CP_Settings_Fill(WHITE);
    CP_Settings_TextSize(text_peek_size);
    CP_Font_DrawText("NEXT", text_peek_pos.x, text_peek_pos.y);

    for (int index = 0; index < PEEK_SIZE; ++index) {
        current = &peek_hand[index];

        if (current == piece_held) continue;

        CP_Settings_StrokeWeight(peek_tile_stroke);

        // Render the background square surrounding each piece
        CP_Settings_Fill(MENU_BLACK);
        CP_Settings_Stroke(current->piece.color);
        CP_Graphics_DrawRect(current->pos.x, current->pos.y, peek_slot_length, peek_slot_length);

        // Settings for tile rendering
        CP_Settings_Fill(current->piece.color);
        CP_Settings_Stroke(current->piece.color_stroke);
        // Render each tile in the Tetris Piece
        float* x_screen_length = &current->piece.x_screen_length;
        float* y_screen_length = &current->piece.y_screen_length;
        for (int index_x = 0; index_x < SHAPE_BOUNDS; ++index_x) {
            for (int index_y = 0; index_y < SHAPE_BOUNDS; ++index_y) {
                if (current->piece.shape[index_x][index_y]) {
                    CP_Graphics_DrawRect(current->piece.draw_pos.x + index_x * *x_screen_length, current->piece.draw_pos.y + index_y * *y_screen_length, *x_screen_length, *y_screen_length);
                }
            }
        }
    }

    // Render the carried piece last so that it is on top of everything
    if (is_piece_held) {
        current = piece_held;
        CP_Settings_StrokeWeight(hand_tile_stroke);

        // Render the background square surrounding each piece
        CP_Settings_Fill(MENU_BLACK);
        CP_Settings_Stroke(current->piece.color);
        CP_Graphics_DrawRect(current->pos.x, current->pos.y, hand_slot_length, hand_slot_length);

        // Settings for tile rendering
        CP_Settings_Fill(current->piece.color);
        CP_Settings_Stroke(current->piece.color_stroke);
        // Render each tile in the Tetris Piece
        float* x_screen_length = &current->piece.x_screen_length;
        float* y_screen_length = &current->piece.y_screen_length;
        for (int index_x = 0; index_x < SHAPE_BOUNDS; ++index_x) {
            for (int index_y = 0; index_y < SHAPE_BOUNDS; ++index_y) {
                if (current->piece.shape[index_x][index_y]) {
                    CP_Graphics_DrawRect(current->piece.draw_pos.x + index_x * *x_screen_length, current->piece.draw_pos.y + index_y * *y_screen_length, *x_screen_length, *y_screen_length);
                }
            }
        }
    }
}

//______________________________________________________________
// More initialization functions

/*______________________________________________________________
@brief Recalculate the position scaling factors and stores them
*/
void RecalculateHandRenderPositions(void) {
    //______________________________________________________________
    // Initialize player hand factors
    hand_total_height = (float)CP_System_GetWindowHeight() * .15f; // The hand queue takes up 15% of the height
    hand_bottom_buffer = hand_total_height * .2f; // The bottom buffer takes up 10% of the hand height

    float total_width = (float)CP_System_GetWindowWidth();
    hand_total_length = total_width * .65f; // The hand queue takes up 65% of the width
    hand_left_buffer = hand_total_length * .1f; // The hand edge buffers takes up 10% of the hand queue
    hand_slot_length = min(hand_total_height, (hand_total_length - hand_left_buffer) * .9f / HAND_SIZE); // The length of each slot depends on whether height or 90% of the hand queue width is shorter
    hand_slot_spacing = (hand_total_length - hand_left_buffer - (hand_slot_length * HAND_SIZE)) / HAND_SIZE; // The spacing of each slot is the remaining space

    // If the spacing between slots are too wide (more than 33% the length of a slot), add more padding to the left
    if ((hand_slot_spacing - hand_slot_length / 3.0f) > 0) {
        hand_left_extra_buffer = (hand_slot_spacing - hand_slot_length / 3.0f) * HAND_SIZE;
        hand_slot_spacing = hand_slot_length / 3.0f;
    }

    hand_tile_length = hand_slot_length / SHAPE_BOUNDS; // How big each tile piece of a Tetris Piece is
    hand_tile_stroke = hand_tile_length * 0.15f; // The stroke of each tile is 15% of the width of the tile

    //______________________________________________________________
    // Initialize peek hand factors
    peek_total_length = total_width - hand_total_length; // The peek queue takes up the remaining width
    peek_right_buffer = peek_total_length * .25f; // The peek edge buffer takes up 25% of the peek queue
    peek_slot_length = min(hand_total_height * .6f, (peek_total_length - peek_right_buffer) * .9f / PEEK_SIZE); // The length of each slot depends on whether height or 90% of the peek queue width is shorter;
    peek_slot_spacing = ((peek_total_length - peek_right_buffer) - peek_slot_length * PEEK_SIZE) / (PEEK_SIZE - 1); // The spacing of each slot is the remaining space

    // If the spacing between slots are too wide (more than 33% the length of a slot), reduce the spacing
    if ((peek_slot_spacing - peek_slot_length / 3.0f) > 0) {
        peek_slot_spacing = peek_slot_length / 3.0f;
    }

    peek_tile_length = peek_slot_length / SHAPE_BOUNDS; // How big each tile piece of a Tetris Piece is
    peek_tile_stroke = peek_tile_length * 0.15f; // The stroke of each tile is 15% of the width of the tile

    text_peek_pos = CP_Vector_Set(hand_total_length, (float)CP_System_GetWindowHeight() - hand_total_height - hand_bottom_buffer);
    text_peek_size = hand_total_height * 0.25f;

    //______________________________________________________________
    // Update the positions
    PlayerHandSlot* current;
    for (int index = 0; index < HAND_SIZE; ++index) {
        current = &hand[index];
        current->pos.x = hand_left_buffer + hand_left_extra_buffer + (hand_slot_length + hand_slot_spacing) * index;
        current->pos.y = (float)CP_System_GetWindowHeight() - hand_slot_length - hand_bottom_buffer;
        current->piece.draw_pos.x = current->pos.x + (SHAPE_BOUNDS - current->piece.x_length) / 2.0f * hand_tile_length;
        current->piece.draw_pos.y = current->pos.y + (SHAPE_BOUNDS - current->piece.y_length) / 2.0f * hand_tile_length;
        current->piece.x_screen_length = hand_tile_length;
        current->piece.y_screen_length = hand_tile_length;
    }

    for (int index = 0; index < PEEK_SIZE; ++index) {
        current = &peek_hand[index];
        current->pos.x = hand_total_length + (peek_slot_length + peek_slot_spacing) * index;
        current->pos.y = (float)CP_System_GetWindowHeight() - peek_slot_length - hand_bottom_buffer;
        current->piece.draw_pos.x = current->pos.x + (SHAPE_BOUNDS - current->piece.x_length) / 2.0f * peek_tile_length;
        current->piece.draw_pos.y = current->pos.y + (SHAPE_BOUNDS - current->piece.y_length) / 2.0f * peek_tile_length;
        current->piece.x_screen_length = peek_tile_length;
        current->piece.y_screen_length = peek_tile_length;
    }
}

//______________________________________________________________
// Player interaction functions

/*______________________________________________________________
@brief When a Tetris Piece is dropped onto the grid, it has been played.

@param int - Which piece was played
*/
void PlayPiece(int played_index) {
    if (played_index == -1) return;

    // JARRETT TODO: Insert the interaction of the piece with the grid here

    // Remove the piece from the player's hand, and shift the pieces behind fowards
    for (int index = played_index; index < HAND_SIZE - 1; ++index) {
        hand[index] = hand[index + 1];
    }

    // Move the next piece from the peek queue into the hand
    hand[HAND_SIZE - 1] = peek_hand[0];

    // Shift the peek queue piece foward
    for (int index = 0; index < PEEK_SIZE - 1; ++index) {
        peek_hand[index] = peek_hand[index + 1];
    }

    // Draw another piece from the queue
    hand[PEEK_SIZE - 1].piece = DrawFromBag();
}
