#include "Soundsystem.h"
#if __cplusplus >= 202002L
#include <ranges>
#endif// #if __cplusplus >= 202002L

#include "Utils/Log.h"
#include "Utils/Random.h"

static const std::string Soundsystem_MusicStr = "music";
static const std::string Soundsystem_GlobalStr = "global";

/**
 * @brief Changes the current music track to a new one, either randomly or sequentially.
 */
void Soundsystem::change_music()
{
	LOG_TRACE("change music called");
	LOG_TRACE("current music: {}", m_current_music);
	const int m_sounds_size = static_cast<signed>(m_sounds_meta_data[Soundsystem_MusicStr].size()) - 1;
	LOG_TRACE("amount of music sounds available: {}", m_sounds_size);
	sf::Sound& music_sound = m_sounds[Soundsystem_MusicStr][0].first.back();
	if (m_sounds_size == 0)
	{
		LOG_INFO("only 1 sound available");
		music_sound.play();
		return;
	}

	int new_current_music;
#ifdef NO_SEQUENTIAL_MUSIC
	new_current_music = Utils::Random::UInt(0, m_sounds_size);
#else
	new_current_music = m_current_music + 1;
	LOG_TRACE("next music id: {}", new_current_music);
	if (new_current_music > m_sounds_size)
	{
		new_current_music = 0;
	}
#endif
	LOG_TRACE("change buffer to that of the new music");
	music_sound.setBuffer(m_sounds_meta_data[Soundsystem_MusicStr][new_current_music].buffer);
	LOG_TRACE("playing music");
	music_sound.play();
	LOG_TRACE("current_music changed");
	m_current_music = new_current_music;

}

/**
 * @brief Validates whether the given group ID exists.
 * @param input The group ID to validate.
 * @return True if the group ID is valid, false otherwise.
 */
bool Soundsystem::validate_group_id(int input) const
{
	LOG_TRACE("Validating if group id: {} exists", input);
	// Check if the group ID is valid and mapped
	if (!m_mapping.contains(input))
		return false;
	return true;
}

/**
 * @brief Validates whether the given group name exists in both the sounds and buffer maps.
 * @param input The group name to validate.
 * @return True if the group name exists, false otherwise.
 */
bool Soundsystem::validate_group_id(const std::string& input) const
{
	LOG_TRACE("Validating if group id: {} exists", input);
	// Check if the group ID is valid and mapped
	if (!m_sounds.contains(input) || !m_sounds_meta_data.contains(input))
		return false;
	return true;
}

size_t Soundsystem::internal_get_group_size(const std::string& group_id) const
{
	if (!m_sounds.contains(group_id)) 
	{
		LOG_WARN("group_id: {} has no sounds",group_id);
		return -1;
	}
	return m_sounds.at(group_id).size();
}

/**
 * @brief Adds a sound to a group, with optional positioning.
 * @param group_id The id of the group.
 * @param sound_id The sound ID.
 * @param pos The 2D position of the sound.
 * @param use_positioning Whether to apply positioning to the sound.
 * @note the group ID and sound ID are based on the order you added them via add_group and load_buffer respectedly.
 * Audio spatialization also only works with mono audio.
 */
void Soundsystem::internal_add_sound(int group_id, int sound_id, sf::Vector3f pos, bool use_positioning)
{
	LOG_TRACE("internal add sound called with: group_id: {} sound_id: {} pos: {{x: {} y: {}}} use_positioning: {}", group_id, sound_id, pos.x, pos.y, use_positioning);
	if (!validate_group_id(group_id))
	{
		LOG_ERROR("Group id: {} doesnt exist", group_id);
		return;
	}
	LOG_TRACE("forwarding arguments to internal_add_sound");
	internal_add_sound(m_mapping[group_id], sound_id, pos, use_positioning);
}

/**
 * @brief Adds a sound to a group, with optional positioning.
 * @param group_id The name of the group.
 * @param sound_id The sound ID.
 * @param pos The 2D position of the sound.
 * @param use_positioning Whether to apply positioning to the sound.
 * @note the sound ID is based on the order you added them via load_buffer.
 * Audio spatialization also only works with mono audio.
 */
void Soundsystem::internal_add_sound(const std::string& group_id, int sound_id, sf::Vector3f pos, bool use_positioning)
{
	LOG_TRACE("internal add sound called with: group_id: {} sound_id: {} pos: {{x: {} y: {}}} use_positioning: {}", group_id, sound_id, pos.x, pos.y, use_positioning);
	LOG_TRACE("checking if new sounds can be added");
	if(m_stop_playing_sounds)
	{
		LOG_INFO("sounds cant be added while stop_playing sounds is true");
		return;
	}

	LOG_TRACE("checking if too many sounds are currently playing");
	LOG_TRACE("amount of currently playing sounds: {}", s_current_playing_sounds);
	if (s_current_playing_sounds >= 256)
	{
		LOG_WARN("too many sounds: {}", s_current_playing_sounds);
		return;
	}
	// Check if the group is "music" because music is handled seperatly
	if (group_id == Soundsystem_MusicStr || group_id == Soundsystem_GlobalStr)
	{
		LOG_INFO("You cant add sounds to the global group!");
		LOG_INFO("Sound addition to the music group is handled seperatly!");
		LOG_INFO("the given id is: {}", group_id);
		return;
	}
	// Check if the group has associated sounds and buffers
	if (!validate_group_id(group_id))
	{
		LOG_WARN("Group_id {} has no associated sounds or buffers", group_id);
		return;
	}
	// Check if the sound ID is within the valid range
	if (sound_id < 0 || sound_id >= m_sounds_meta_data[group_id].size())
	{
		LOG_WARN("Sound id is outside the valid range from: 0 to {}", m_sounds_meta_data[group_id].size());
		LOG_WARN("Sound id is: {}", sound_id);
		return;
	}
	//wir holen uns die sound queue di zu der sound gruppe und sound id passt
	auto& current_sounds = m_sounds[group_id][sound_id].first;
	//wir holen uns ob dieser sound priorität hat
	const bool has_priority = m_sounds[group_id][sound_id].second;
	LOG_TRACE("has_priority: {}", has_priority);
	//globale lautstärke holen
	const float allgemeiner_volume = m_volumes[m_mapping[-1]];
	LOG_TRACE("global volume: {}", allgemeiner_volume);
	//gruppen lautstärke holen
	const float group_volume = m_volumes[group_id];
	LOG_TRACE("group volume: {}", group_volume);

	// If no priority or sound list is empty (with priority), add and play the sound
	if (!has_priority || (current_sounds.empty() && has_priority))
	{
		LOG_TRACE("all checks passed adding and playing sound");
		//sound buffer holen
		const Soundmetadata& metadata = m_sounds_meta_data[group_id][sound_id];
		const sf::SoundBuffer& sound_buffer = metadata.buffer;
		//sound wird zu queue hinzugefügt und soundbuffer wird hinzugefügt
		current_sounds.emplace_back(sound_buffer);
		//der gerade hinzugefügter sound wird geholt
		sf::Sound& current_sound = current_sounds.back();

		//sagen ob der sound relativ zum hörer ist
		current_sound.setRelativeToListener(!use_positioning);

		//sound wird abgespielt
		current_sound.play();
		//lautstärke wird angepasst
		current_sound.setVolume(allgemeiner_volume * group_volume / 100.f);
		//soll der sound 3d audio sein
		if (use_positioning)
		{
			//setze die position für den sound
			if(m_use_tile_size && m_tilesize > 0.f)
			{
				current_sound.setPosition(pos.x / m_tilesize, pos.y / m_tilesize, pos.z / m_tilesize);
			}
			else
			{
				current_sound.setPosition(pos.x, pos.y, pos.z);
			}
			//distanz ab wann der sound nicht mehr komplett gehört wird
			current_sound.setMinDistance(metadata.mindistance);
			//faktor festlegen wie stark die lautstärke verändert wird wenn man weiter weg ist
			current_sound.setAttenuation(metadata.attenuation);
		}
		//menge der momentan spielenden sounds erhöhen
		s_current_playing_sounds++;
		LOG_TRACE("now {} sounds are playing", s_current_playing_sounds);
		m_new_data = true;
	}
}

/**
 * @brief Sets the volume for a specific group.
 * @param volume The volume level to set (0.0f to 100.0f).
 * @param id The id of the group.
 * @note The input value will be clamped if it exceeds the bounds. Users should
 * be aware that if their value is outside this range, it will be automatically
 * adjusted to fit within 0.0f to 100.0f.
 */
void Soundsystem::internal_set_volume(float volume, const int id)
{
	LOG_TRACE("internal_set_volume called with parameters: volumes: {} id: {}", volume, id );
	if(!validate_group_id(id))
	{
		LOG_WARN("Group id: {} doesnt exist", id);
		return;
	}
	LOG_TRACE("forwarding arguments to internal_set_volume");
	internal_set_volume(volume, m_mapping[id]);
}

/**
 * @brief Sets the volume for a specific group.
 * @param volume The volume level to set (0.0f to 100.0f).
 * @param group_id The name of the group.
 * @note The input value will be clamped if it exceeds the bounds. Users should
 * be aware that if their value is outside this range, it will be automatically
 * adjusted to fit within 0.0f to 100.0f.
 */
void Soundsystem::internal_set_volume(float volume, const std::string& group_id)
{
	LOG_TRACE("internal_set_volume called with parameters: volume: {} group_id: {}", volume, group_id);
	// Sicherstellen, dass die Lautstärke im gültigen Bereich [0.0, 100.0] liegt
	if (volume < 0.0f || volume > 100.0f)
	{
		LOG_INFO("Volume: {} is not in between the limit of 0.f to 100.f. The value will be clamped", volume);
		volume = std::clamp(volume, 0.f, 100.f);
	}

	// Überprüfen, ob die ID in der Zuordnung existiert
	if (!validate_group_id(group_id))
	{
		LOG_WARN("Group: {} doesnt exist", group_id);
		return;
	}

	const float delta = abs(m_volumes[group_id] - volume);
	LOG_TRACE("difference between the volume of the group: {} and the new volume: {} is: {}", m_volumes[group_id], volume, delta);
	if (delta < 1e-3f)
		return;
	LOG_TRACE("added volume: {} to group_id: {}", volume, group_id);
	m_new_volumes[group_id] = volume;
	m_new_data = true;
}

/**
 * @brief Increments the volume for a specific group	.
 * @param increase The amount to increment the volume by (0.0f to 100.0f).
 * @param id The id of the group.
 * @note The input value will be clamped if it exceeds the bounds. Users should
 * be aware that if their value is outside this range, it will be automatically
 * adjusted to fit within 0.0f to 100.0f.
 */
void Soundsystem::internal_increment_volume(float increase, int id)
{
	LOG_TRACE("internal_increment_volume called with parameters: increase: {} id: {}", increase, id);
	if (!validate_group_id(id))
	{
		LOG_WARN("Group id: {} doesnt exist", id);
		return;
	}
	LOG_TRACE("forwarding arguments to internal_set_volume");
	internal_set_volume(increase + m_volumes[m_mapping[id]], id);
}

/**
 * @brief Increments the volume for a specific group.
 * @param increase The amount to increment the volume by (0.0f to 100.0f).
 * @param group_id The name of the group.
 * @note The input value will be clamped if it exceeds the bounds. Users should
 * be aware that if their value is outside this range, it will be automatically
 * adjusted to fit within 0.0f to 100.0f.
 */
void Soundsystem::internal_increment_volume(float increase, const std::string& group_id)
{
	LOG_TRACE("internal_increment_volume called with parameters: increase: {} id: {}", increase, group_id);
	if (!validate_group_id(group_id))
	{
		LOG_WARN("Group id: {} doesnt exist", group_id);
		return;
	}
	LOG_TRACE("forwarding arguments to internal_set_volume");
	internal_set_volume(increase + m_volumes[group_id], group_id);
}

void Soundsystem::internal_set_volumes(const std::unordered_map<std::string, float>& volumes)
{
	LOG_TRACE("internal_set_volumes called with volumes which size is: {}", volumes.size());

	if (volumes.empty())
	{
		return;
	}

	if(GET_LOG_LEVEL == LOG_LEVEL_TRACE)
	{
		LOG_TRACE("incoming volumes: ");
		for (auto& volume_groups : volumes)
		{
			LOG_TRACE("group: {} volume: {}",volume_groups.first,volume_groups.second);
		}
		LOG_TRACE("already existing volumes");
		for (auto& volume_groups : m_volumes)
		{
			LOG_TRACE("group: {} volume: {}", volume_groups.first, volume_groups.second);
		}
	}

	auto set_volume_for_group = [&](float end_lautstaerke, const std::string& group_name) -> void
	{
		if(end_lautstaerke > 100.f || end_lautstaerke < 0)
		{
			LOG_INFO("end_lautstaerke zu hoch / niedrig : {}", end_lautstaerke);
			end_lautstaerke = std::clamp(end_lautstaerke, 0.f, 100.f);
		}

		for (auto& sounds : m_sounds[group_name] | std::views::keys)
		{
			for (sf::Sound& sound : sounds)
			{
				sound.setVolume(end_lautstaerke);
			}
		}
	};

	bool global_sound_changed = false;
	if(volumes.contains(Soundsystem_GlobalStr))
	{
		m_volumes[Soundsystem_GlobalStr] = volumes.at(Soundsystem_GlobalStr);
		LOG_TRACE("global sound changed to: {}", volumes.at(Soundsystem_GlobalStr));
		global_sound_changed = true;
	}
	const float global_music_lvl = m_volumes[Soundsystem_GlobalStr];

	for (auto& volume_groups : m_volumes)
	{
		if (volume_groups.first == Soundsystem_GlobalStr)
			continue;
		if(!volumes.contains(volume_groups.first))
		{
			if (global_sound_changed)
			{
				const float final_volume = (volume_groups.second * global_music_lvl) / 100.f;
				LOG_TRACE("group volume: {}", volume_groups.second);
				LOG_TRACE("final_volume: {}", final_volume);
				set_volume_for_group(final_volume,volume_groups.first);
			}
			continue;
		}
		const float group_volume = volumes.at(volume_groups.first);
		const float final_volume = (group_volume * global_music_lvl) / 100.f;
		LOG_TRACE("group volume: {}", group_volume);
		LOG_TRACE("final_volume: {}", final_volume);
		volume_groups.second = group_volume;
		set_volume_for_group(final_volume,volume_groups.first);
	}
}

void Soundsystem::internal_load_buffer(const std::string& location, bool priority, const std::string& group_id,
	const Soundmetadata& metadata)
{
	LOG_TRACE("internal_load_buffer called with parameters: location: {} priority: {} group: {} metadata: {{min_distance: {} attenuation: {}}}",
		location, priority, group_id, metadata.mindistance, metadata.attenuation);
	if (!validate_group_id(group_id))
	{
		LOG_WARN("Group id: {} doesnt exist", group_id);
		return;
	}

	if (group_id == Soundsystem_GlobalStr)
	{
		LOG_WARN("You cant add buffers or sounds to the global group");
		return;
	}
	sf::SoundBuffer temp_buffer;
	temp_buffer.loadFromFile(location);
	Soundmetadata temp_metadata{metadata};

	temp_metadata.buffer = temp_buffer;
	if(temp_metadata.attenuation == 0.f)
	{
		temp_metadata.attenuation = 1.f;
	}

	m_sounds_meta_data[group_id].push_back(temp_metadata);
	std::deque<sf::Sound> temp_vector;
	if (group_id == Soundsystem_MusicStr)
	{
		if (m_sounds[group_id].empty())
		{
			m_sounds[group_id].emplace_back(temp_vector, true);
		}
	}
	else 
	{
		m_sounds[group_id].emplace_back(temp_vector, priority);
	}
}

void Soundsystem::internal_load_buffer(const std::string& location, bool priority, int group_id,
	const Soundmetadata& metadata)
{
	LOG_TRACE("load_buffer called with parameters: location: {} priority: {} group: {} metadata: {{min_distance: {} attenuation: {}}}",
		location, priority, group_id, metadata.mindistance, metadata.attenuation);
	if (!validate_group_id(group_id))
	{
		LOG_INFO("Group id: {} doesnt exist", group_id);
		return;
	}
	LOG_TRACE("forwarding arguments to internal_set_volume");
	internal_load_buffer(location, priority, m_mapping[group_id], metadata);
}

/**
 * @brief Constructor for the Soundsystem class.
 * @note Adds Groups "global" and "music"
 */
Soundsystem::Soundsystem(float tilesize, bool use_tile_size) : m_use_tile_size(use_tile_size), m_tilesize(tilesize)
{
	sf::Listener::setPosition(0.f,0.f,0.f);
	add_group(Soundsystem_GlobalStr);
	LOG_TRACE("added global group");
	add_group(Soundsystem_MusicStr);
	LOG_INFO("starting sound thread");
	start_thread();
}

void Soundsystem::run()
{
	while (true) {
		std::unique_lock lock(m_mutex);

		m_cv.wait(lock, [this]()
		{
			if (m_should_play_music && !m_stop && !m_stop_playing_sounds)
			{
				if (music())
					return true;
			}
			return m_new_data || m_stop || !m_new_volumes.empty();
		});

		if (m_stop)
		{
			LOG_INFO("shutting down soundsystem");
			break;  // Exit the thread loop if stopping is requested
		}

		//update volumes
		internal_set_volumes(m_new_volumes);

		if(m_stop_playing_sounds != m_prev_stop_playing_sounds)
		{
			if (m_stop_playing_sounds)
				pause_all(true);
			else
				play_all();
			m_prev_stop_playing_sounds = m_stop_playing_sounds;
		}

		if (m_prev_should_play_music != m_should_play_music) 
		{
			if (!m_should_play_music)
				pause_music();
			else if(!m_stop_playing_sounds)
				play_music();

			m_prev_should_play_music = m_should_play_music;
		}

		// Reset the condition flag (i.e., new data has been handled)
		m_new_data = false;
		m_new_volumes.clear();
	}
}

/**
 * @brief Initializes the sound system singleton instance.
 * @note Constructor Adds Groups "global" and "music". should be called before get_instance
 */
void Soundsystem::init(float tilesize, bool use_tile_size)
{
	LOG_TRACE("init called");
	if (!s_instance) 
	{
		LOG_INFO("Instance doesnt exist creating a new one");
		s_instance = new Soundsystem(tilesize, use_tile_size);
	}
}

/**
 * @brief Retrieves the singleton instance of the Soundsystem.
 * @return A pointer to the Soundsystem instance.
 * @note call init before get_instance otherwise you get a nullptr
 */
Soundsystem* Soundsystem::get_instance()
{
	LOG_TRACE("get_instance called");
	return s_instance;
}

/**
 * @brief Deletes the singleton instance of the Soundsystem.
 */
void Soundsystem::delete_instance()
{
	LOG_TRACE("delete_instance called");
	delete s_instance;
}

/**
 * @brief Destructor for the Soundsystem class. Clears all resources.
 */
Soundsystem::~Soundsystem()
{
	LOG_TRACE("destructor called");
	LOG_INFO("clearing sounds");
	clear_all();
	LOG_INFO("stopping thread");
	stop_thread();  // Ensure the thread is stopped when the object is destroyed
	LOG_INFO("joining thread");
	if (m_thread.joinable())
	{
		m_thread.join();
		LOG_INFO("thread joined");
	}
	else 
	{
		LOG_CRITICAL("unable to join thread");
	}
}

void Soundsystem::start_thread()
{
	LOG_TRACE("called start_thread");
	m_stop = false;
	m_thread = std::thread(&Soundsystem::run, this);
}

void Soundsystem::stop_thread()
{
	LOG_TRACE("stop_thread called");
	{
		std::lock_guard lock(m_mutex);
		m_stop = true;
		m_new_data = true;
	}
	LOG_TRACE("condition variable notified");
	m_cv.notify_all();  // Notify the thread to stop
}


/**
 * @brief Retrieves a list of the current volume levels for all groups.
 * @return A vector of float values representing the volumes.
 * @note Index 0 is the global volume, Index 1 to n are your own sounds.
 */
std::unordered_map<std::string, float> Soundsystem::get_volumes() const
{
	LOG_TRACE("get_volumes called");
	return m_volumes;
}

/**
 * @brief Loads a sound buffer from a file and adds it to the specified group.
 * @param location The file path of the sound buffer to load.
 * @param priority If true, this sound has priority over others.
 * @param group_id The group to which this sound belongs.
 */
void Soundsystem::load_buffer(const std::string& location, bool priority, const std::string& group_id)
{
	LOG_TRACE("load_buffer called with parameters: location: {} priority: {} group: {}", location, priority, group_id);
	std::lock_guard lock(m_mutex);
	internal_load_buffer(location,priority, group_id,{});
}

void Soundsystem::load_buffer(const std::string& location, bool priority, int group_id)
{
	LOG_TRACE("load_buffer called with parameters: location: {} priority: {} group: {}", location, priority, group_id);
	std::lock_guard lock(m_mutex);
	internal_load_buffer(location, priority, group_id, {});
}

void Soundsystem::load_buffer(const std::string& location, bool priority, const std::string& group_id,
	const Soundmetadata& metadata)
{
	LOG_TRACE("load_buffer called with parameters: location: {} priority: {} group: {} metadata: {{min_distance: {} attenuation: {}}}",
		location, priority, group_id, metadata.mindistance, metadata.attenuation);
	std::lock_guard lock(m_mutex);
	internal_load_buffer(location, priority, group_id, metadata);
}

void Soundsystem::load_buffer(const std::string& location, bool priority, int group_id, const Soundmetadata& metadata)
{
	LOG_TRACE("load_buffer called with parameters: location: {} priority: {} group: {} metadata: {{min_distance: {} attenuation: {}}}",
		location, priority, group_id, metadata.mindistance, metadata.attenuation);
	std::lock_guard lock(m_mutex);
	internal_load_buffer(location, priority, group_id, metadata);
}

void Soundsystem::set_should_play_music(bool should_play_music)
{
	LOG_TRACE("set_should_play_music called with parameters: should_play_music: {}", should_play_music);
	std::lock_guard lock(m_mutex);
	LOG_TRACE("checking if the data changed");
	m_new_data = m_should_play_music != should_play_music || m_new_data;
	LOG_TRACE("should play music changed {}", m_should_play_music != should_play_music);
	m_should_play_music = should_play_music;
}

bool Soundsystem::is_should_play_music() const
{
	return m_should_play_music;
}

void Soundsystem::set_use_tile_size(const bool i_use_tile_size)
{
	std::lock_guard lock(m_mutex);
	m_use_tile_size = i_use_tile_size;
}

bool Soundsystem::is_use_tile_size() const
{
	return m_use_tile_size;
}

void Soundsystem::set_tilesize(const float i_tilesize)
{
	std::lock_guard lock(m_mutex);
	m_tilesize = i_tilesize;
}


float Soundsystem::get_tilesize() const
{
	return m_tilesize;
}

void Soundsystem::set_stop_playing_sounds(const bool i_stop_adding_new_sounds)
{
	std::lock_guard lock(m_mutex);
	m_new_data = m_stop_playing_sounds != i_stop_adding_new_sounds || m_new_data;
	m_stop_playing_sounds = i_stop_adding_new_sounds;
}

bool Soundsystem::get_stop_playing_sounds() const
{
	return m_stop_playing_sounds;
}

size_t Soundsystem::get_group_size(int group_id) const
{
	if (!validate_group_id(group_id)) 
	{
		LOG_WARN("group_id: {} doesnt exist", group_id);
		return -1;
	}
	return internal_get_group_size(m_mapping.at(group_id));
}

size_t Soundsystem::get_group_size(const std::string& group_id) const
{
	return internal_get_group_size(group_id);
}

std::unordered_map<std::string, size_t> Soundsystem::get_group_names() const
{
	std::unordered_map<std::string, size_t> return_val;
	for(auto& [name,sounds] : m_sounds)
	{
		return_val[name] = sounds.size();
	}
	return return_val;
}


/**
 * @brief Adds a new group to the Soundsystem.
 * @param group The name of the new group.
 */
void Soundsystem::add_group(const std::string& group)
{
	LOG_TRACE("add_group called with parameters: group: {}",group);
	std::lock_guard lock(m_mutex);
	if(validate_group_id(group))
	{
		LOG_WARN("Group: {} already exists", group);
		return;
	}
	m_sounds_meta_data.insert({group,{}});
	m_sounds.insert({ group, {} });
	if (group != Soundsystem_GlobalStr)
		m_mapping[static_cast<signed>(m_sounds.size()) - 1] = group;
	else
		m_mapping[-1] = group;
	m_volumes.insert({ group ,100.f });
	LOG_TRACE("succesfully added group: {}", group);
}

/**
 * @brief Adds a sound to a group by its ID.
 * @param group_id The group ID.
 * @param sound_id The sound ID.
 * @note the group ID and sound ID are based on the order
 * you added them via add_group and load_buffer respectedly.
 */
void Soundsystem::add_sound(const int group_id, const int sound_id)
{
	LOG_TRACE("add_sound called with parameters: group_id: {} sound_id: {}", group_id, sound_id);
	std::lock_guard lock(m_mutex);
	LOG_TRACE("forwarding arguments to internal_add_sound");
	internal_add_sound(group_id, sound_id, {}, false);
}

/**
 * @brief Adds a sound to a group by its name.
 * @param group_id The name of the group.
 * @param sound_id The sound ID.
 * @note the sound ID is based on the order you added sounds to the group via load_buffer
 */
void Soundsystem::add_sound(const std::string& group_id, const int sound_id)
{
	LOG_TRACE("add_sound called with parameters: group_id: {} sound_id: {}", group_id, sound_id);
	std::lock_guard lock(m_mutex);
	LOG_TRACE("forwarding arguments to internal_add_sound");
	internal_add_sound(group_id, sound_id, {}, false);
}

/**
 * @brief Adds a sound with positional data to a group by its ID.
 * @param group_id The group ID.
 * @param sound_id The sound ID.
 * @param pos The 2D position of the sound.
 * @note the group ID and sound ID are based on the order you added them via add_group and load_buffer respectedly.
 * Audio spatialization also only works with mono audio.
 */
void Soundsystem::add_sound(const int group_id, const int sound_id, const sf::Vector3f pos)
{
	LOG_TRACE("add_sound called with parameters: group_id: {} sound_id: {} pos: {{x: {}, y: {}}}", group_id, sound_id, pos.x, pos.y);
	std::lock_guard lock(m_mutex);
	LOG_TRACE("forwarding arguments to internal_add_sound");
	internal_add_sound(group_id, sound_id, pos, true);
}

/**
 * @brief Adds a sound with positional data to a group by its name.
 * @param group_id The name of the group.
 * @param sound_id The sound ID.
 * @param pos The 2D position of the sound.
 * @note the sound ID is based on the order you added them via load_buffer.
 * Audio spatialization also only works with mono audio.
 */
void Soundsystem::add_sound(const std::string& group_id, const int sound_id, const sf::Vector3f pos)
{
	LOG_TRACE("add_sound called with parameters: group_id: {} sound_id: {} pos: {{x: {}, y: {}}}",group_id,sound_id,pos.x,pos.y);
	std::lock_guard lock(m_mutex);
	LOG_TRACE("forwarding arguments to internal_add_sound");
	internal_add_sound(group_id, sound_id, pos, true);
}

void Soundsystem::play_music()
{
	LOG_TRACE("play music called");
	if (m_sounds[Soundsystem_MusicStr].empty() || m_sounds_meta_data[Soundsystem_MusicStr].empty())
		return;
	auto& music_sounds = m_sounds[Soundsystem_MusicStr][0].first;
	if (music_sounds.empty())
	{
		music_sounds.emplace_back(m_sounds_meta_data[Soundsystem_MusicStr][0].buffer);
		music_sounds.back().setVolume(m_volumes[Soundsystem_GlobalStr] * m_volumes[Soundsystem_MusicStr] / 100);
	}
	if (m_current_music == -1)
	{
		music_sounds.back().setBuffer(m_sounds_meta_data[Soundsystem_MusicStr][0].buffer);
		s_current_playing_sounds++;
	}
	sf::Sound& music_sound = music_sounds.back();
	music_sound.play();
}

void Soundsystem::pause_music()
{
	LOG_TRACE("pause music called");
	if (m_sounds[Soundsystem_MusicStr].empty() || m_current_music == -1 || m_sounds[Soundsystem_MusicStr][0].first.empty())
		return;
	m_sounds[Soundsystem_MusicStr][0].first.back().pause();
}

void Soundsystem::clear_music()
{
	LOG_TRACE("clear music called");
	if (m_sounds[Soundsystem_MusicStr].empty())
		return;
	m_sounds[Soundsystem_MusicStr].clear();
	m_sounds_meta_data[Soundsystem_MusicStr].clear();
	s_current_playing_sounds--;

}


/**
 * @brief Plays music and plays new music once the old one stopped
 */
bool Soundsystem::music()
{
	LOG_TRACE("music called");
	if (m_sounds[Soundsystem_MusicStr].empty())
	{
		LOG_WARN("no music buffers exist");
		return false;
	}
	auto& music_sounds = m_sounds[Soundsystem_MusicStr][0].first;
	if (m_current_music == -1)
	{
		if (music_sounds.empty())
		{
			music_sounds.emplace_back(m_sounds_meta_data[Soundsystem_MusicStr][0].buffer);
			music_sounds.back().setVolume(m_volumes[Soundsystem_MusicStr] * m_volumes[Soundsystem_MusicStr] / 100);
		}
		music_sounds.back().setBuffer(m_sounds_meta_data[Soundsystem_MusicStr][0].buffer);
		music_sounds.back().play();

		m_current_music = 0;

		s_current_playing_sounds++;
	}
	if (music_sounds.back().getStatus() == sf::SoundSource::Stopped)
	{
		change_music();
		return true;
	}
	return false;

}

/**
 * @brief Deletes all currently stopped sounds.
 */
void Soundsystem::cleanup()
{
	LOG_TRACE("cleanup called");
	int counter = 0;
	auto manual_erase = [&](std::deque<sf::Sound>& sounds, bool& priority) -> void {
		if (!priority)
		{
			for (int i = 0; i < sounds.size();i++)
			{
				if (sounds.front().getStatus() == sf::SoundSource::Stopped)
				{
					sounds.pop_front();
					s_current_playing_sounds--;
					counter++;
				}
				else
				{
					break;
				}
			}
		}
	};

	for (auto& all_sounds : m_sounds | std::views::values)
	{
		for (auto& [sounds, priority] : all_sounds)
		{
			manual_erase(sounds, priority);
		}
	}
	LOG_TRACE("removed {} stopped sounds ", counter);
}	

/**
 * @brief Deletes all currently playing sounds.
 */
void Soundsystem::delete_sounds()
{
	LOG_TRACE("delete_sounds called");
	for (auto& all_sounds : m_sounds | std::views::values)
	{
		for (auto& sounds : all_sounds | std::views::keys)
		{
			s_current_playing_sounds -= sounds.size();
			sounds.clear();
		}
	}
	m_current_music = -1;
}

/**
 * @brief Pauses all currently playing sounds in the system.
 * @param ignore_priority should sounds of groups with priority also be stopped.
 */
void Soundsystem::pause_all(const bool ignore_priority)
{
	LOG_TRACE("pause_all called");
	auto pause = [&](auto& sounds, const bool& priority)
	{
		if (!priority || ignore_priority)
		{
			for (sf::Sound& sound : sounds)
			{
				if (sound.getStatus() != sf::SoundSource::Stopped)
				{
					sound.pause();
				}
			}
		}
	};
	for (auto& all_sounds : m_sounds | std::views::values)
	{
		for (auto& [sounds, priority] : all_sounds)
		{
			pause(sounds, priority);
		}
	}

}

/**
 * @brief Plays all currently non stopped sounds in the system.
 */
void Soundsystem::play_all()
{
	LOG_TRACE("play_all called");
	auto play = [](std::deque<sf::Sound>& sounds)
	{
		for (sf::Sound& sound : sounds)
		{
			if (sound.getStatus() != sf::SoundSource::Stopped)
			{
				sound.play();
			}
		}
	};

	for (auto& [group_name, all_sounds] : m_sounds )
	{
		if(group_name == Soundsystem_MusicStr && !m_should_play_music)
			continue;
		for (auto& sounds : all_sounds | std::views::keys)
		{
			play(sounds);
		}
	}
}

/**
 * @brief Clears all the sounds from the system, stopping playback and releasing resources.
 */
void Soundsystem::clear_all()
{
	LOG_TRACE("clear all called");
	delete_sounds();
	m_sounds.clear();
	m_sounds_meta_data.clear();
}

/**
 * @brief Sets the volume for a specific group.
 * @param volume The volume level to set (0.0f to 100.0f).
 * @param group_id The id of the group.
 * @note The input value will be clamped if it exceeds the bounds. Users should 
 * be aware that if their value is outside this range, it will be automatically
 * adjusted to fit within 0.0f to 100.0f.
 */
void Soundsystem::set_volume(float volume, const int group_id)
{
	LOG_TRACE("set_volume called with with parameters: volume: {} group_id: {}", volume, group_id);
	std::lock_guard lock(m_mutex);
	LOG_TRACE("forwarding arguments to internal_set_volume");
	internal_set_volume(volume, group_id);
}

/**
 * @brief Sets the volume for a specific group.
 * @param volume The volume level to set (0.0f to 100.0f).
 * @param group_id The name of the group.
 * @note The input value will be clamped if it exceeds the bounds. Users should 
 * be aware that if their value is outside this range, it will be automatically
 * adjusted to fit within 0.0f to 100.0f.
 */
void Soundsystem::set_volume(float volume, const std::string& group_id)
{
	LOG_TRACE("set_volume called with with parameters: volume: {} group_id: {}", volume, group_id);
	std::lock_guard lock(m_mutex);
	LOG_TRACE("forwarding arguments to internal_set_volume");
	internal_set_volume(volume, group_id);
}

void Soundsystem::set_volume(const std::unordered_map<std::string, float>& volumes)
{
	LOG_TRACE("set_volume called with volumes of size: {}", volumes.size());
	std::lock_guard lock(m_mutex);
	for (const auto& [group_id, volume] : volumes)
	{
		LOG_TRACE("forwarding arguments to internal_set_volume");
		internal_set_volume(volume, group_id);
	}
}


/**
 * @brief Increments the volume for a specific group.
 * @param increase The amount to increment the volume by (0.0f to 100.0f).
 * @param group_id The id of the group.
 * @note The input value will be clamped if it exceeds the bounds. Users should 
 * be aware that if their value is outside this range, it will be automatically
 * adjusted to fit within 0.0f to 100.0f.
 */
void Soundsystem::increment_volume(float increase, int group_id)
{
	LOG_TRACE("increment_volume called with parameters: increase: {} group_id: {}", increase, group_id);
	std::lock_guard lock(m_mutex);
	LOG_TRACE("forwarding arguments to internal_increment_volume");
	internal_increment_volume(increase, group_id);
}

/**
 * @brief Increments the volume for a specific group.
 * @param increase The amount to increment the volume by (0.0f to 100.0f).
 * @param group_id The name of the group.
 * @note The input value will be clamped if it exceeds the bounds. Users should
 * be aware that if their value is outside this range, it will be automatically
 * adjusted to fit within 0.0f to 100.0f.
 */
void Soundsystem::increment_volume(float increase, const std::string& group_id)
{
	LOG_TRACE("increment_volume called with parameters: increase: {} group_id: {}", increase, group_id);
	std::lock_guard lock(m_mutex);
	LOG_TRACE("forwarding arguments to internal_increment_volume");
	internal_increment_volume(increase ,group_id);
}

/**
 * @brief Decrements the volume for a specific group.
 * @param decrease The amount to decrease the volume by (0.0f to 100.0f).
 * @param group_id The id of the group.
 * @note The input value will be clamped if it exceeds the bounds. Users should 
 * be aware that if their value is outside this range, it will be automatically
 * adjusted to fit within 0.0f to 100.0f.
 */
void Soundsystem::decrement_volume(float decrease, int group_id)
{
	LOG_TRACE("decrement_volume called with parameters: decrease: {} group_id: {}", decrease, group_id);
	std::lock_guard lock(m_mutex);
	LOG_TRACE("forwarding arguments to internal_increment_volume");
	internal_increment_volume(-decrease,group_id);
}

/**
 * @brief Decrements the volume for a specific group.
 * @param decrease The amount to decrease the volume by (0.0f to 100.0f).
 * @param group_id The name of the group.
 * @note The input value will be clamped if it exceeds the bounds. Users should
 * be aware that if their value is outside this range, it will be automatically
 * adjusted to fit within 0.0f to 100.0f.
 */
void Soundsystem::decrement_volume(float decrease, const std::string& group_id)
{
	LOG_TRACE("decrement_volume called with parameters: decrease: {} group_id: {}", decrease, group_id);
	std::lock_guard lock(m_mutex);
	LOG_TRACE("forwarding arguments to internal_increment_volume");
	internal_increment_volume(-decrease, group_id);
}

void Soundsystem::update()
{
	LOG_TRACE("update called");
	{

		std::lock_guard lock(m_mutex);
		LOG_TRACE("cleaning up stopped sounds");
		cleanup();
	}
	LOG_TRACE("notifying the condition variable");
	m_cv.notify_all();
}
