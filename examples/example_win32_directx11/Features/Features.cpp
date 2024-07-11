#include "Features.hpp"
#include "../Configs/ConfigManager.hpp"

#define PARSE_ESP_TABLE(config, table_name, esp_instance) {					 \
	auto table = toml::find(config.unwrap(), table_name);				     \
																		     \
	esp_instance.enabled = table.at("enabled").as_boolean();			     \
																		     \
	esp_instance.mode = table.at("mode").as_string();					     \
	esp_instance.box_color.color = table.at("color").as_array();		     \
																		     \
	esp_instance.outline = table.at("outline").as_boolean();				 \
	esp_instance.outline_color.color = table.at("outline_color").as_array(); \
																		     \
	esp_instance.filling = table.at("filling").as_boolean();				 \
	esp_instance.filling_color.color = table.at("filling_color").as_array(); \
}																		     \

#define PARSE_ON_ENABLE_MODULE_TABLE(config, table_name, on_enable_module_instance) { \
	auto table = toml::find(config.unwrap(), table_name);					          \
	on_enable_module_instance.enabled = table.at("enabled").as_boolean();	          \
}																					  \

namespace Features {
    const toml::result<toml::value, std::vector<toml::error_info>> Visuals::config = toml::try_parse(Configs::Visuals::config_path);

    void format_boolean(std::stringstream& result, const std::string& key, const toml::value& value) {
        result << key << " = " << (value.as_boolean() ? "true" : "false");

        auto comments = value.comments();
        if (!comments.empty())
            result << " " << comments.front();

        result << "\n";
    }

    void format_string(std::stringstream& result, const std::string& key, const toml::value& value) {
        result << key << " = \"" << value.as_string() << "\"";

        auto comments = value.comments();
        if (!comments.empty())
            result << " " << comments.front();

        result << "\n";
    }

    void format_array(std::stringstream& result, const std::string& key, const toml::value& value) {
        result << key << " = [";

        for (const auto& elem : value.as_array())
            result << elem.as_integer() << ", ";

        result.seekp(-2, std::ios::end); // remove trailing comma and space
        result << "]";

        auto comments = value.comments();
        if (!comments.empty())
            result << " " << comments.front(); // add comment on the same line

        result << "\n";
    }

    std::string format_table(const toml::table& table) {
        std::stringstream result;

        for (const auto& [key, value] : table) {
            if (key == "PlayerESP" || key == "ChestESP") {
                result << "[" << key << "]\n";
                format_boolean(result, "enabled", value.at("enabled"));

                result << "\n";
                format_string(result, "mode ", value.at("mode")); // add 1 space to "mode" so that align "=" symbols
                format_array(result, "color", value.at("color"));

                result << "\n";
                format_boolean(result, "outline      ", value.at("outline")); // 6 spaces
                format_array(result, "outline_color", value.at("outline_color"));

                result << "\n";
                format_boolean(result, "filling      ", value.at("filling")); // 6 spaces
                format_array(result, "filling_color", value.at("filling_color"));
            }
            else if (key == "Chams") {
                result << "[" << key << "]\n";
                format_boolean(result, "players", value.at("players"));
                format_boolean(result, "chests ", value.at("chests")); // 1 space
            }
            else if (key == "NameTags") {
                result << "[" << key << "]\n";
                format_boolean(result, "enabled", value.at("enabled"));
            }

            result << "\n";
        }

        return result.str();
    }

    void Visuals::OnEnableModule::update_enabled() const {
        auto& some_module = toml::find(config.unwrap(), this->module_name);
        const_cast<toml::type_config::boolean_type&>(some_module.at("enabled").as_boolean()) = this->enabled;
        Configs::Visuals::UpdateSettings(format_table(config.unwrap().as_table()));
    }

    void Visuals::ESP::update_mode() const {
        auto& esp_module = toml::find(config.unwrap(), this->module_name);
        const_cast<toml::type_config::string_type&>(esp_module.at("mode").as_string()) = this->mode;
        Configs::Visuals::UpdateSettings(format_table(config.unwrap().as_table()));
    }

    void Visuals::ESP::update_box_color() const {
        auto& esp_module = toml::find(config.unwrap(), this->module_name);
        const_cast<toml::value::array_type&>(esp_module.at("color").as_array()) = this->box_color.color;
        Configs::Visuals::UpdateSettings(format_table(config.unwrap().as_table()));
    }

    void Visuals::ESP::update_outline() const {
        auto& esp_module = toml::find(config.unwrap(), this->module_name);
        const_cast<toml::type_config::boolean_type&>(esp_module.at("outline").as_boolean()) = this->outline;
        Configs::Visuals::UpdateSettings(format_table(config.unwrap().as_table()));
    }

    void Visuals::ESP::update_outline_color() const {
        auto& esp_module = toml::find(config.unwrap(), this->module_name);
        const_cast<toml::value::array_type&>(esp_module.at("outline_color").as_array()) = this->outline_color.color;
        Configs::Visuals::UpdateSettings(format_table(config.unwrap().as_table()));
    }

    void Visuals::ESP::update_filling() const {
        auto& esp_module = toml::find(config.unwrap(), this->module_name);
        const_cast<toml::type_config::boolean_type&>(esp_module.at("filling").as_boolean()) = this->filling;
        Configs::Visuals::UpdateSettings(format_table(config.unwrap().as_table()));
    }

    void Visuals::ESP::update_filling_color() const {
        auto& esp_module = toml::find(config.unwrap(), this->module_name);
        const_cast<toml::value::array_type&>(esp_module.at("filling_color").as_array()) = this->filling_color.color;
        Configs::Visuals::UpdateSettings(format_table(config.unwrap().as_table()));
    }

    void Visuals::Chams::update_players() {
        auto& chams = toml::find(config.unwrap(), "Chams");
        const_cast<toml::type_config::boolean_type&>(chams.at("players").as_boolean()) = this->players;
        Configs::Visuals::UpdateSettings(format_table(config.unwrap().as_table()));
    }

    void Visuals::Chams::update_chests() {
        auto& chams = toml::find(config.unwrap(), "Chams");
        const_cast<toml::type_config::boolean_type&>(chams.at("chests").as_boolean()) = this->chests;
        Configs::Visuals::UpdateSettings(format_table(config.unwrap().as_table()));
    }

    void Visuals::LoadSettings() {
        PARSE_ESP_TABLE(config, "PlayerESP", this->player_esp);
        PARSE_ESP_TABLE(config, "ChestESP", this->chest_esp);

        auto chams = toml::find(config.unwrap(), "Chams");

        this->chams.players = chams.at("players").as_boolean();
        this->chams.chests = chams.at("chests").as_boolean();

        PARSE_ON_ENABLE_MODULE_TABLE(config, "NameTags", this->name_tags);
    }
}
