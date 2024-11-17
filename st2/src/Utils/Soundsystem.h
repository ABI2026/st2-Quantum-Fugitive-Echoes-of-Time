#pragma once
#include <deque>
#include <mutex>
#include <unordered_map>
#include <SFML/Audio.hpp>

class Soundsystem 
{
public:

	struct Soundmetadata
	{
		float attenuation{ 0 };
		float mindistance{ 0 };
		Soundmetadata() = default;
		Soundmetadata(const float i_attenuation, const float i_mindistance)
			:attenuation(i_attenuation), mindistance(i_mindistance) {}
	private:
		friend class Soundsystem;
		sf::SoundBuffer buffer;
	};


	//constructors and destructors
public:

	~Soundsystem();
	Soundsystem(float tilesize, bool use_tile_size);
	Soundsystem() = delete;
	Soundsystem(const Soundsystem&) = delete;
	Soundsystem operator=(const Soundsystem&) = delete;
	Soundsystem(Soundsystem&&) = delete;
	Soundsystem operator=(Soundsystem&&) = delete;



	//getter and setter
public:

	[[nodiscard]] std::unordered_map<std::string, float> get_volumes() const;

	void set_should_play_music(bool should_play_music);

	[[nodiscard]] bool is_should_play_music() const;

	void set_use_tile_size(bool i_use_tile_size);

	[[nodiscard]] bool is_use_tile_size() const;

	void set_tilesize(float i_tilesize);

	[[nodiscard]] float get_tilesize() const;

	void set_stop_playing_sounds(bool i_stop_adding_new_sounds);

	[[nodiscard]] bool get_stop_playing_sounds() const;

	[[nodiscard]] size_t get_group_size(int group_id) const;

	[[nodiscard]] size_t get_group_size(const std::string& group_id) const;

	[[nodiscard]] std::unordered_map<std::string, size_t> get_group_names() const;

public:
	void add_group(const std::string& group);

	void load_buffer(const std::string& location, bool only_one_sound, const std::string& group_id);

	void load_buffer(const std::string& location, bool only_one_sound, int group_id);

	void load_buffer(const std::string& location, bool only_one_sound, const std::string& group_id, const Soundmetadata& metadata);

	void load_buffer(const std::string& location, bool only_one_sound, int group_id, const Soundmetadata& metadata);


	void add_sound(const std::string& group_id, int sound_id);

	void add_sound(int group_id, int sound_id);

	void add_sound(const std::string& group_id, int sound_id, sf::Vector3f pos);

	void add_sound(int group_id, int sound_id, sf::Vector3f pos);


	void set_volume(float volume, int id);

	void set_volume(float volume, const std::string& group_id);

	void set_volume(const std::unordered_map<std::string, float>& volumes);

	void increment_volume(float increase, int group_id);

	void increment_volume(float increase, const std::string& group_id);

	void decrement_volume(float decrease, int group_id);

	void decrement_volume(float decrease, const std::string& group_id);


	void update();

	//run method which runs on another thread
private:
	void run();

	void start_thread();

	void stop_thread();

private:
	void change_music();

	[[nodiscard]] bool validate_group_id(int input) const;

	[[nodiscard]] bool validate_group_id(const std::string& input) const;

	[[nodiscard]] size_t internal_get_group_size(const std::string& group_id) const;

	//private methods to have one location for logic
private:
	void internal_add_sound(int group_id, int sound_id, sf::Vector3f pos, bool use_positioning);

	void internal_add_sound(const std::string& group_id, int sound_id, sf::Vector3f pos, bool use_positioning);

	void internal_set_volume(float volume, int id);

	void internal_set_volume(float volume, const std::string& id);

	void internal_increment_volume(float increase, int id);

	void internal_increment_volume(float increase, const std::string& group_id);

	void internal_set_volumes(const std::unordered_map<std::string, float>& volumes);

	void internal_load_buffer(const std::string& location, bool only_one_sound, const std::string& group, const Soundmetadata& metadata);

	void internal_load_buffer(const std::string& location, bool only_one_sound, int group, const Soundmetadata& metadata);





private:

	void play_music();

	void pause_music();

	void clear_music();

	bool music();

	void cleanup();

	void delete_sounds();

	void pause_all();

	void play_all();

	void clear_all();


	//attributes for handling of multithreading
private:
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::thread m_thread;

	//attributes to know if something changed
private:
	bool m_new_data = false;
	bool m_stop = false;
	bool m_should_play_music = false;
	bool m_prev_should_play_music = true;
	std::unordered_map<std::string, float> m_new_volumes;

	bool m_use_tile_size = false;
	float m_tilesize;
	bool m_stop_playing_sounds = false;
	bool m_prev_stop_playing_sounds = false;

	//sound data and mappings
private:
	typedef std::pair<std::deque<sf::Sound>, bool> sounds;
	typedef std::deque<sounds> sound_group;

	std::unordered_map<int, std::string> m_mapping;
	std::unordered_map<std::string, float> m_volumes;
	std::unordered_map<std::string, std::deque<Soundmetadata>> m_sounds_meta_data;
	std::unordered_map<std::string, sound_group> m_sounds;

	int m_current_music = -1;

	inline static size_t s_current_playing_sounds = 0;

};
