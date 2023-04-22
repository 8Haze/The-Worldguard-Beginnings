#include "resources.h"

RM_Textures::RM_Textures()
{
    using namespace std::string_literals;
    using namespace RESOURCE_ID;

    storage.emplace(TEXTURES::MENU_BG, path + "menu_background.png"s);
    storage.emplace(TEXTURES::MAIN_MENU_LOGO, path + "main_menu_logo.png"s);
    storage.emplace(TEXTURES::GUI_BUTTON, path + "button.png"s);
    storage.emplace(TEXTURES::GUI_DIALOGUE_BOX, path + "dialogue_box.png"s);
    storage.emplace(TEXTURES::GUI_TEXT_FIELD, path + "text_field.png"s);
    storage.emplace(TEXTURES::GUI_USER_LEVEL_LIST, path + "user_level_list.png"s);
    storage.emplace(TEXTURES::GUI_BUTTON_USER_LEVEL_LIST, path + "button_user_level_list.png"s);
    storage.emplace(TEXTURES::GUI_CAMPAIGN_LEVEL_PANEL, path + "campaign_level_panel.png"s);
    storage.emplace(TEXTURES::THEMES_LOGOS, path + "themes_logos.png"s);
    storage.emplace(TEXTURES::TOOLBELT, path + "toolbelt.png"s);
    storage.emplace(TEXTURES::TOOLBELT_INFO_PANEL, path + "toolbelt_info_panel.png"s);
    storage.emplace(TEXTURES::LEVEL_CREATOR_GRID, path + "level_creator_grid.png"s);
    storage.emplace(TEXTURES::LCOS_TILE, path + "lcos_tile.png"s);
    storage.emplace(TEXTURES::LCOS_TILE_LAYER, path + "lcos_tile_layer.png"s);
    storage.emplace(TEXTURES::LCOS_ITEM, path + "lcos_item.png"s);
    storage.emplace(TEXTURES::LCOS_FOOD, path + "lcos_food.png"s);
    storage.emplace(TEXTURES::LCOS_GEM, path + "lcos_gem.png"s);
    storage.emplace(TEXTURES::LCOS_COIN, path + "lcos_coin.png"s);
    storage.emplace(TEXTURES::LCOS_LIFEUP, path + "lcos_lifeup.png"s);
    storage.emplace(TEXTURES::LCOS_ENTITY, path + "lcos_entity.png"s);
    storage.emplace(TEXTURES::LCOS_LIGHT, path + "lcos_light.png"s);

    storage.emplace(TEXTURES::FOOD, path + "food.png"s);
    storage.emplace(TEXTURES::GEM, path + "gem.png"s);
    storage.emplace(TEXTURES::COIN, path + "coin.png"s);
    storage.emplace(TEXTURES::LIFEUP, path + "lifeup.png"s);
    storage.emplace(TEXTURES::ITEM_FADEOUT, path + "item_fadeout.png"s);

    storage.emplace(TEXTURES::BULLET, path + "bullet.png"s);
    storage.emplace(TEXTURES::BULLET_FADEOUT, path + "bullet_fadeout.png"s);
    storage.emplace(TEXTURES::WAVE, path + "wave.png"s);
    storage.emplace(TEXTURES::WAVE_FADEOUT, path + "wave_fadeout.png"s);
    storage.emplace(TEXTURES::MASS, path + "mass.png"s);
    storage.emplace(TEXTURES::MASS_FADEOUT  , path + "mass_fadeout.png"s);
    storage.emplace(TEXTURES::ROCK, path + "rock.png"s);
    storage.emplace(TEXTURES::ROCK_CRACK, path + "rock_crack.png"s);
    storage.emplace(TEXTURES::ICE_CHUNK, path + "ice_chunk.png"s);
    storage.emplace(TEXTURES::ICE_CHUNK_CRACK, path + "ice_chunk_crack.png"s);
    storage.emplace(TEXTURES::BALL, path + "ball.png"s);
    storage.emplace(TEXTURES::BALL_CRACK, path + "ball_crack.png"s);
    storage.emplace(TEXTURES::BUBBLE, path + "bubble.png"s);
    storage.emplace(TEXTURES::BUBBLE_POP, path + "bubble_pop.png"s);
    storage.emplace(TEXTURES::BOOMERANG, path + "boomerang.png"s);
    storage.emplace(TEXTURES::THORN_SPEAR, path + "thorn_spear.png"s);
    storage.emplace(TEXTURES::BIONIC_RAY, path + "bionic_ray.png"s);
    storage.emplace(TEXTURES::ELECTRIC_GATE, path + "electric_gate.png"s);
    storage.emplace(TEXTURES::HEALING_BLAST, path + "healing_blast.png"s);
    storage.emplace(TEXTURES::PRESSURE_BLAST, path + "pressure_blast.png"s);
    storage.emplace(TEXTURES::FLAMETHROWER_FLAME, path + "flamethrower_flame.png"s);

    storage.emplace(TEXTURES::VERDANT_TILESET, path + "verdant_tileset.png"s);
    storage.emplace(TEXTURES::WATER_TILESET, path + "water_tileset.png"s);
    storage.emplace(TEXTURES::FIRE_TILESET, path + "fire_tileset.png"s);
    storage.emplace(TEXTURES::ELECTRIC_TILESET, path + "electric_tileset.png"s);
    storage.emplace(TEXTURES::WIND_TILESET, path + "wind_tileset.png"s);
    storage.emplace(TEXTURES::GROUND_TILESET, path + "ground_tileset.png"s);
    storage.emplace(TEXTURES::LIFEFORCE_TILESET, path + "lifeforce_tileset.png"s);
    storage.emplace(TEXTURES::ICE_TILESET, path + "ice_tileset.png"s);

    storage.emplace(TEXTURES::ANIMATED_TILESET, path + "animated_tileset.png"s);
    storage.emplace(TEXTURES::INTERACTIVE_TILESET, path + "interactive_tileset.png"s);

    storage.emplace(TEXTURES::VERDANT_BACKGROUND, path + "verdant_background.png"s);
    storage.emplace(TEXTURES::WATER_BACKGROUND, path + "water_background.png"s);
    storage.emplace(TEXTURES::FIRE_BACKGROUND, path + "fire_background.png"s);
    storage.emplace(TEXTURES::ELECTRIC_BACKGROUND, path + "electric_background.png"s);
    storage.emplace(TEXTURES::WIND_BACKGROUND, path + "wind_background.png"s);
    storage.emplace(TEXTURES::GROUND_BACKGROUND, path + "ground_background.png"s);
    storage.emplace(TEXTURES::LIFEFORCE_BACKGROUND, path + "lifeforce_background.png"s);
    storage.emplace(TEXTURES::ICE_BACKGROUND, path + "ice_background.png"s);

    storage.emplace(TEXTURES::TRAMPOLINE, path + "trampoline.png"s);
    storage.emplace(TEXTURES::CANNON, path + "cannon.png"s);
    storage.emplace(TEXTURES::BOUNCER, path + "bouncer.png"s);
    storage.emplace(TEXTURES::WIND_BLASTER, path + "wind_blaster.png"s);
    storage.emplace(TEXTURES::MINE, path + "mine.png"s);
    storage.emplace(TEXTURES::LIVING_SPIKES, path + "living_spikes.png"s);
    storage.emplace(TEXTURES::THORN, path + "thorn.png"s);
    storage.emplace(TEXTURES::THORN_SPEAR_ROOT, path + "thorn_spear_root.png"s);
    storage.emplace(TEXTURES::FALLING_HAZARD, path + "falling_hazard.png"s);
    storage.emplace(TEXTURES::ROCK_DISPOSER, path + "rock_disposer.png"s);
    storage.emplace(TEXTURES::ICE_LAUNCHER, path + "ice_launcher.png"s);
    storage.emplace(TEXTURES::BUBBLE_MACHINE, path + "bubble_machine.png"s);
    storage.emplace(TEXTURES::HEALING_PLANT, path + "healing_plant.png"s);
    storage.emplace(TEXTURES::PRESSURE_ORB, path + "pressure_orb.png"s);
    storage.emplace(TEXTURES::BIONIC_RAY_DEVICE, path + "bionic_ray_device.png"s);
    storage.emplace(TEXTURES::ELECTRIC_GATE_DEVICE, path + "electric_gate_device.png"s);
    storage.emplace(TEXTURES::FLAMETHROWER, path + "flamethrower.png"s);
    storage.emplace(TEXTURES::ROAMING_TRAMPOLINE, path + "roaming_trampoline.png"s);
    storage.emplace(TEXTURES::SCOUTER, path + "scouter.png"s);

    storage.emplace(TEXTURES::WARRIOR, path + "warrior.png"s);
    storage.emplace(TEXTURES::CASTER, path + "caster.png"s);
    storage.emplace(TEXTURES::GOLEM, path + "golem.png"s);
    storage.emplace(TEXTURES::ELEMENTAL, path + "elemental.png"s);
    storage.emplace(TEXTURES::JUNGLE_BOOMERANGER, path + "jungle_boomeranger.png"s);
    storage.emplace(TEXTURES::AQUAVEIL_WEAVER, path + "aquaveil_weaver.png"s);
    storage.emplace(TEXTURES::SCORCHER, path + "scorcher.png"s);
    storage.emplace(TEXTURES::ENERGIZED_GALVANIZER, path + "energized_galvanizer.png"s);
    storage.emplace(TEXTURES::AERIAL_BOMBARDIER, path + "aerial_bombardier.png"s);
    storage.emplace(TEXTURES::ROCK_HURLER, path + "rock_hurler.png"s);
    storage.emplace(TEXTURES::CRIMSON_LASHER, path + "crimson_lasher.png"s);
    storage.emplace(TEXTURES::ICEBANER, path + "icebaner.png"s);

    storage.emplace(TEXTURES::POWER_CELLS, path + "power_cells.png"s);
    storage.emplace(TEXTURES::EXPLOSION_32, path + "explosion_32.png"s);
    storage.emplace(TEXTURES::EXPLOSION_48, path + "explosion_48.png"s);
    storage.emplace(TEXTURES::SCORCHER_AFTERMATH, path + "scorcher_aftermath.png"s);
    storage.emplace(TEXTURES::PLAYER_LIFEBAR_FRAME, path + "player_lifebar_frame.png"s);
    storage.emplace(TEXTURES::PLAYER_LIFEBAR_FILLING, path + "player_lifebar_filling.png"s);
    storage.emplace(TEXTURES::TEMPERATURE_BAR_FRAME, path + "temperature_bar_frame.png"s);
    storage.emplace(TEXTURES::TEMPERATURE_BAR_FILLING, path + "temperature_bar_filling.png"s);

    storage.emplace(TEXTURES::NAZAR, path + "nazar.png"s);
}

sf::Texture& RM_Textures::load_resource(std::string file, RESOURCE_ID::TEXTURES id)
{
    sf::Image image;
    image.loadFromFile(file);
    image.createMaskFromColor(sf::Color::White);

    auto texture = std::make_unique<sf::Texture>();
    texture->loadFromImage(image);

    if (id == RESOURCE_ID::TEXTURES::MENU_BG)
        texture->setRepeated(true);

    auto res = cache.emplace(id, std::move(texture));
    return *res.first->second;
}

RM_Sounds::RM_Sounds()
{
    using namespace std::string_literals;
    using namespace RESOURCE_ID;

    storage.emplace(SOUNDS::GUI_BUTTON_SELECT, path + "button_switch.ogg"s);
    storage.emplace(SOUNDS::GUI_BUTTON_EXECUTE, path + "button_execute.ogg"s);
    storage.emplace(SOUNDS::GUI_TEXT_ENTERED, path + "button_execute.ogg"s);
}

sf::SoundBuffer& RM_Sounds::load_resource(std::string file, RESOURCE_ID::SOUNDS id)
{
    auto sbuffer = std::make_unique<sf::SoundBuffer>();
    sbuffer->loadFromFile(file);

    auto res = cache.emplace(id, std::move(sbuffer));
    return *res.first->second;
}

RM_Fonts::RM_Fonts()
{
    using namespace std::string_literals;
    using namespace RESOURCE_ID;

    storage.emplace(FONTS::MAIN, path + "main.ttf"s);
}

sf::Font& RM_Fonts::load_resource(std::string file, RESOURCE_ID::FONTS id)
{
    auto font = std::make_unique<sf::Font>();
    font->loadFromFile(file);

    auto res = cache.emplace(id, std::move(font));
    return *res.first->second;
}