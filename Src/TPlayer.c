/*!
@file	  TPlayer.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     26/10/2022
@brief    This source file
________________________________________________________________________________________________________*/

#include "ColorTable.h"

#include "TManager.h" 
#include "TPlayer.h" 

PlayerHandSlot hand[HAND_SIZE];
PlayerHandSlot peek_hand[PEEK_SIZE];

CP_Vector text_peek_pos;

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
}

// JARRETT TODO: Finish pick up piece stub function
void PickUpPiece(void) {
    // JARRETT TODO: If piece is dropped onto the grid, get the corresponding pos and do something
    // PlayPiece();

    // JARRETT TODO: If piece is dropped onto hand, return it and carry on
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
        CP_Settings_Stroke(WHITE);
        CP_Graphics_DrawRect(current->pos.x, current->pos.y, hand_slot_length, hand_slot_length);

        // Settings for tile rendering
        CP_Settings_Fill(WHITE);
        CP_Settings_Stroke(BLACK);
        // Render each tile in the Tetris Piece
        for (int index_x = 0; index_x < SHAPE_BOUNDS; ++index_x) {
            for (int index_y = 0; index_y < SHAPE_BOUNDS; ++index_y) {
                if (current->piece.shape[index_x][index_y]) {
                    CP_Graphics_DrawRect(current->piece.draw_pos.x + index_x * hand_tile_length, current->piece.draw_pos.y + index_y * hand_tile_length, hand_tile_length, hand_tile_length);
                }
            }
        }
    }

    //______________________________________________________________
    // Render each piece in the peek queue & the "NEXT" text
    CP_Settings_TextSize(text_peek_size);
    CP_Font_DrawText("NEXT", text_peek_pos.x, text_peek_pos.y);

    for (int index = 0; index < PEEK_SIZE; ++index) {
        current = &peek_hand[index];

        CP_Settings_StrokeWeight(peek_tile_stroke);

        // Render the background square surrounding each piece
        CP_Settings_Fill(MENU_BLACK);
        CP_Settings_Stroke(WHITE);
        CP_Graphics_DrawRect(current->pos.x, current->pos.y, peek_slot_length, peek_slot_length);

        // Settings for tile rendering
        CP_Settings_Fill(WHITE);
        CP_Settings_Stroke(BLACK);
        // Render each tile in the Tetris Piece
        for (int index_x = 0; index_x < SHAPE_BOUNDS; ++index_x) {
            for (int index_y = 0; index_y < SHAPE_BOUNDS; ++index_y) {
                if (current->piece.shape[index_x][index_y]) {
                    CP_Graphics_DrawRect(current->piece.draw_pos.x + index_x * peek_tile_length, current->piece.draw_pos.y + index_y * peek_tile_length, peek_tile_length, peek_tile_length);
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
    }

    for (int index = 0; index < PEEK_SIZE; ++index) {
        current = &peek_hand[index];
        current->pos.x = hand_total_length + (peek_slot_length + peek_slot_spacing) * index;
        current->pos.y = (float)CP_System_GetWindowHeight() - peek_slot_length - hand_bottom_buffer;
        current->piece.draw_pos.x = current->pos.x + (SHAPE_BOUNDS - current->piece.x_length) / 2.0f * peek_tile_length;
        current->piece.draw_pos.y = current->pos.y + (SHAPE_BOUNDS - current->piece.y_length) / 2.0f * peek_tile_length;
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
