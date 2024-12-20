#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

#define MAX_NAME_LENGTH 60

// Song structure
struct song_t {
    char title[MAX_NAME_LENGTH];
    char artist[MAX_NAME_LENGTH];
    int duration;
    song_t *next; // Pointer to the next song
};

// Playlist structure
struct playlist_t {
    char name[MAX_NAME_LENGTH];
    song_t *head; // Pointer to the first song in the linked list
};

// Function to create a playlist
void create_playlist(playlist_t *playlist, const char *name) {
    strncpy(playlist->name, name, MAX_NAME_LENGTH);
    playlist->head = nullptr; // Initialize the linked list as empty
}

// Function to add a song to the playlist
void add_song_to_playlist(playlist_t *playlist, const char *title, const char *artist, int duration) {
    song_t *new_song = new song_t; // Dynamically allocate memory for a new song
    strncpy(new_song->title, title, MAX_NAME_LENGTH);
    strncpy(new_song->artist, artist, MAX_NAME_LENGTH);
    new_song->duration = duration;
    new_song->next = playlist->head; // Insert at the beginning of the list
    playlist->head = new_song;
}

// Function to display the playlist
void display_playlist(const playlist_t *playlist) {
    cout << "Playlist: " << playlist->name << endl;
    song_t *current = playlist->head;
    int index = 1;
    while (current) {
        cout << "Song " << index++ << ": " << current->title << " by " << current->artist
             << " (" << current->duration << " sec)" << endl;
        current = current->next;
    }
}

// Function to calculate total duration of the playlist
int calculate_total_duration(const playlist_t *playlist) {
    int total_duration = 0;
    song_t *current = playlist->head;
    while (current) {
        total_duration += current->duration;
        current = current->next;
    }
    return total_duration;
}

// Function to save the playlist to a file
void save_playlist(const playlist_t *playlist, const char *filename) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error: Unable to open file for saving." << endl;
        return;
    }
    file.write(playlist->name, MAX_NAME_LENGTH); // Save playlist name
    song_t *current = playlist->head;
    while (current) {
        file.write(current->title, MAX_NAME_LENGTH);
        file.write(current->artist, MAX_NAME_LENGTH);
        file.write(reinterpret_cast<char *>(&current->duration), sizeof(current->duration));
        current = current->next;
    }
    file.close();
    cout << "Playlist saved to " << filename << endl;
}

// Function to read a playlist from a file
void read_playlist(playlist_t *playlist, const char *filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error: Unable to open file for reading." << endl;
        return;
    }
    create_playlist(playlist, ""); // Initialize playlist
    file.read(playlist->name, MAX_NAME_LENGTH); // Read playlist name
    while (file.peek() != EOF) {
        song_t *new_song = new song_t;
        file.read(new_song->title, MAX_NAME_LENGTH);
        file.read(new_song->artist, MAX_NAME_LENGTH);
        file.read(reinterpret_cast<char *>(&new_song->duration), sizeof(new_song->duration));
        new_song->next = playlist->head;
        playlist->head = new_song;
    }
    file.close();
    cout << "Playlist loaded from " << filename << endl;
}

// Main function
int main() {
    playlist_t playlist;

    // Create a playlist
    create_playlist(&playlist, "Songs I Love");

    // Add songs to the playlist
    add_song_to_playlist(&playlist, "Little Sister", "Queens of the Stone Age", 2713);
    add_song_to_playlist(&playlist, "Dr. Sunshine is Dead", "Will Wood and The Tapeworms", 2713);
    add_song_to_playlist(&playlist, "Stardom", "King Gnu", 2713);

    // Display the playlist
    display_playlist(&playlist);

    // Calculate and display total duration
    cout << "Total duration: " << calculate_total_duration(&playlist) << " seconds" << endl;

    // Save the playlist
    save_playlist(&playlist, "playlist.bin");

    // Load the playlist
    playlist_t loaded_playlist;
    read_playlist(&loaded_playlist, "playlist.bin");

    // Display the loaded playlist
    display_playlist(&loaded_playlist);

    return 0;
}
