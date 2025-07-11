/**
 * @file melodies.c
 * @brief Melodies source file.
 * @author Sistemas Digitales II
 * @date 2024-01-01
 */

/* Includes ------------------------------------------------------------------*/
#include "melodies.h"

/* Melodies ------------------------------------------------------------------*/
// Melody Happy Birthday
#define HAPPY_BIRTHDAY_LENGTH 25 /*!< Happy Birthday melody length */

/**
 * @brief Happy Birthday melody notes.
 *
 * This array contains the frequencies of the notes for the Happy Birthday song.
 * The notes are defined as frequency values in Hertz, and they are arranged in the order they are played in the song.
 */
static const double happy_birthday_notes[HAPPY_BIRTHDAY_LENGTH] = {
    DO4, DO4, RE4, DO4, FA4, MI4, DO4, DO4, RE4, DO4, SOL4, FA4, DO4, DO4, DO5, LA4, FA4, MI4, RE4, LAs4, LAs4, LA4, FA4, SOL4, FA4};

/**
 * @brief Happy Birthday melody durations in miliseconds.
 *
 * This array contains the duration of each note in the Happy Birthday song.
 * The durations are defined in milliseconds, and they are arranged in the order they are played in the song.
 */
static const uint16_t happy_birthday_durations[HAPPY_BIRTHDAY_LENGTH] = {
    300, 100, 400, 400, 400, 800, 300, 100, 400, 400, 400, 800, 300, 100, 400, 400, 400, 400, 400, 300, 100, 400, 400, 400, 800};

/**
 * @brief Happy Birthday melody struct.
 *
 * This struct contains the information of the Happy Birthday melody.
 * It is used to play the melody using the buzzer.
 */
const melody_t happy_birthday_melody = {.p_name = "happy_birthday",
                                        .p_notes = (double *)happy_birthday_notes,
                                        .p_durations = (uint16_t *)happy_birthday_durations,
                                        .melody_length = HAPPY_BIRTHDAY_LENGTH};

// Tetris melody
#define TETRIS_LENGTH 40 /*!< Tetris melody length */

/**
 * @brief Tetris melody notes.
 *
 * This array contains the frequencies of the notes for the Tetris song.
 * The notes are defined as frequency values in Hertz, and they are arranged in the order they are played in the song.
 */
static const double tetris_notes[TETRIS_LENGTH] = {
    MI5, SI4, DO5, RE5, DO5, SI4, LA4, LA4, DO5, MI5, RE5, DO5, SI4, DO5, RE5, MI5, DO5, LA4,
    LA4, LA4, SI4, DO5, RE5, FA4, LA5, SOL5, FA5, MI5, DO5, MI5, RE5, DO5, SI4, SI4, LA4, RE5,
    MI5, DO5, LA4, LA4};

/** 
 * @brief Tetris melody durations in miliseconds.
 *
 * This array contains the duration of each note in the Tetris song.
 * The durations are defined in milliseconds, and they are arranged in the order they are played in the song.
 */
static const uint16_t tetris_durations[TETRIS_LENGTH] = {
    400, 200, 200, 400, 200, 200, 400, 200, 200, 400, 200, 200, 600, 200, 400, 400, 400, 400, 200, 200, 200, 200,
    600, 200, 400, 200, 200, 600, 200, 400, 200, 200, 400, 200, 200, 400, 400, 400, 400, 400};

/**
 * @brief Tetris melody struct.
 * 
 * This struct contains the information of the Tetris melody.
 * It is used to play the melody using the buzzer.
 */
const melody_t tetris_melody = {.p_name = "tetris",
                                .p_notes = (double *)tetris_notes,
                                .p_durations = (uint16_t *)tetris_durations,
                                .melody_length = TETRIS_LENGTH};

// Scale Melody
#define SCALE_MELODY_LENGTH 8   /*!< Scale melody length */

/**
 * @brief Scale melody notes.
 *
 * This array contains the frequencies of the notes for the scale song.
 * The notes are defined as frequency values in Hertz, and they are arranged in the order they are played in the song.
 */
static const double scale_melody_notes[SCALE_MELODY_LENGTH] = {
    DO4, RE4, MI4, FA4, SOL4, LA4, SI4, DO5};

/**
 * @brief Scale melody durations in miliseconds.
 * 
 * This array contains the duration of each note in the scale song.
 * The durations are defined in milliseconds, and they are arranged in the order they are played in the song.
 */
static const uint16_t scale_melody_durations[SCALE_MELODY_LENGTH] = {
    250, 250, 250, 250, 250, 250, 250, 250};

/**
 * @brief Scale melody struct.
 * 
 * This struct contains the information of the scale melody.
 * It is used to play the melody using the buzzer.
 */
const melody_t scale_melody = {.p_name = "scale",
                               .p_notes = (double *)scale_melody_notes,
                               .p_durations = (uint16_t *)scale_melody_durations,
                               .melody_length = SCALE_MELODY_LENGTH};

// Melodies ------------------------------------------------------------------/
// Himno del Real Madrid
#define HIMNO_MADRID_LENGTH 26

/**

@brief Himno del Real Madrid melody notes.
This array contains the frequencies of the notes for the Himno del Real Madrid song.
The notes are defined as frequency values in Hertz, and they are arranged in the order they are played in the song.
*/
static const double himno_madrid_notes[HIMNO_MADRID_LENGTH] = {
MI4, SOL4, LA4, LA4, SOL4, FA4, MI4, RE4, RE4, RE4, MI4, FA4, SOL4, MI4, DO4, RE4, MI4, MI4, RE4, DO4, SI3, RE4, MI4, FA4, MI4, DO4};
/**

@brief Himno del Real Madrid melody durations in miliseconds.
This array contains the duration of each note in the Himno del Real Madrid song.
The durations are defined in milliseconds, and they are arranged in the order they are played in the song.
*/
static const uint16_t himno_madrid_durations[HIMNO_MADRID_LENGTH] = {
400, 400, 800, 400, 400, 800, 400, 400, 800, 400, 400, 400, 800, 400, 400, 400, 800, 400, 400, 800, 400, 400, 800, 400, 400, 800};
/**

@brief Himno del Real Madrid melody struct.
This struct contains the information of the Himno del Real Madrid melody.
It is used to play the melody using the buzzer.
*/
const melody_t himno_madrid_melody = {.p_name = "himno_madrid",
.p_notes = (double *)himno_madrid_notes,
.p_durations = (uint16_t *)himno_madrid_durations,
.melody_length = HIMNO_MADRID_LENGTH};
/* Melodies ------------------------------------------------------------------*/
// Windows Shutdown melody
#define WINDOWS_SHUTDOWN_LENGTH 14 /*!< Windows Shutdown melody length */

/**
 * @brief Windows Shutdown melody notes.
 *
 * This array contains the frequencies of the notes for the Windows Shutdown sound.
 * The notes are defined as frequency values in Hertz, and they are arranged in the order they are played in the song.
 */
static const double windows_shutdown_notes[WINDOWS_SHUTDOWN_LENGTH] = {
    SOL4, MI4, DO4, RE4, DO4, SOL4, MI4, DO4, RE4, DO4, SOL4, MI4, DO4};

/**
 * @brief Windows Shutdown melody durations in miliseconds.
 *
 * This array contains the duration of each note in the Windows Shutdown sound.
 * The durations are defined in milliseconds, and they are arranged in the order they are played in the song.
 */
static const uint16_t windows_shutdown_durations[WINDOWS_SHUTDOWN_LENGTH] = {
    400, 400, 400, 400, 800, 400, 400, 400, 400, 800, 400, 400, 800};

/**
 * @brief Windows Shutdown melody struct.
 *
 * This struct contains the information of the Windows Shutdown melody.
 * It is used to play the melody using the buzzer.
 */
const melody_t windows_shutdown_melody = {.p_name = "windows_shutdown",
                                          .p_notes = (double *)windows_shutdown_notes,
                                          .p_durations = (uint16_t *)windows_shutdown_durations,
                                          .melody_length = WINDOWS_SHUTDOWN_LENGTH};