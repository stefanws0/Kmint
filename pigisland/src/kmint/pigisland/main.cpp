#include "kmint/main.hpp" // voor de main loop
#include "kmint/pigisland/boat.hpp"
#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/pigisland/shark.hpp"
#include "kmint/play.hpp"
#include "kmint/ui.hpp"
#include "kmint/pigisland/wall.hpp"

using namespace kmint;
int main() {
	// een app object is nodig om
	ui::app app{};

  //  maak een venster aan
  ui::window window{app.create_window({1024, 768}, "hello", 0.75)};

	// maak walls aan
	std::vector<pigisland::wall> walls;
	walls.push_back(pigisland::wall{ {16, 112}, {208, 112} });
	walls.push_back(pigisland::wall{ {208, 112}, {208, 80} });
	walls.push_back(pigisland::wall{ {208, 80}, {304, 80} });
	walls.push_back(pigisland::wall{ {304, 80}, {304, 16} });
	walls.push_back(pigisland::wall{ {304, 16}, {880, 16} });
	walls.push_back(pigisland::wall{ {880, 16}, {880, 80} });
	walls.push_back(pigisland::wall{ {880, 80}, {976, 80} });
	walls.push_back(pigisland::wall{ {976, 80}, {976, 144} });
	walls.push_back(pigisland::wall{ {976, 144}, {1008, 144} });
	walls.push_back(pigisland::wall{ {1008, 144}, {1008, 560} });
	walls.push_back(pigisland::wall{ {1008, 560}, {944, 560} });
	walls.push_back(pigisland::wall{ {944, 560}, {944, 528} });
	walls.push_back(pigisland::wall{ {944, 528}, {880, 528} });
	walls.push_back(pigisland::wall{ {880, 528}, {880, 592} });
	walls.push_back(pigisland::wall{ {880, 592}, {816, 592} });
	walls.push_back(pigisland::wall{ {816, 592}, {816, 624} });
	walls.push_back(pigisland::wall{ {816, 624}, {784, 624} });
	walls.push_back(pigisland::wall{ {784, 624}, {784, 752} });
	walls.push_back(pigisland::wall{ {784, 752}, {80, 752} });
	walls.push_back(pigisland::wall{ {80, 752}, {80, 720} });
	walls.push_back(pigisland::wall{ {80, 720}, {208, 720} });
	walls.push_back(pigisland::wall{ {208, 720}, {208, 656} });
	walls.push_back(pigisland::wall{ {208, 656}, {240, 656} });
	walls.push_back(pigisland::wall{ {240, 656}, {240, 528} });
	walls.push_back(pigisland::wall{ {240, 528}, {48, 528} });
	walls.push_back(pigisland::wall{ {48, 528}, {48, 656} });
	walls.push_back(pigisland::wall{ {48, 656}, {16, 656} });
	walls.push_back(pigisland::wall{ {16, 656}, {16, 112} });

	// maak een podium aan
	play::stage s{};

	auto map = pigisland::map();
	map.graph()[0].tagged(true);
	s.build_actor<play::background>(math::size(1024, 768),
		graphics::image{ map.background_image() });
	s.build_actor<play::map_actor>(math::vector2d{ 0.f, 0.f }, map.graph());

	auto& shark = s.build_actor<pigisland::shark>(map.graph());
	auto& boat = s.build_actor<pigisland::boat>(map.graph());

	std::vector<pigisland::pig*> pigs;
	for (auto i = 0; i < 100; ++i)
	{
		auto& pig = s.build_actor<pigisland::pig>(math::vector2d(i * 10.0f, i * 6.0f), i, walls);
		pig.set_boat(boat);
		pig.set_shark(shark);
		pigs.emplace_back(&pig);
		shark.add_pig(pig);
	}
	for (auto pig : pigs)
	{
		auto new_pigs = pigs;
		new_pigs.erase(std::remove(new_pigs.begin(), new_pigs.end(), pig), new_pigs.end());
		pig->set_pigs(new_pigs);
	}

	// Maak een event_source aan (hieruit kun je alle events halen, zoals
	// toetsaanslagen)
	ui::events::event_source event_source{};

	// main_loop stuurt alle actors aan.
	main_loop(s, window, [&](delta_time dt, loop_controls &ctl) {
		// gebruik dt om te kijken hoeveel tijd versterken is
		// sinds de vorige keer dat deze lambda werd aangeroepen
		// loop controls is een object met eigenschappen die je kunt gebruiken om de
		// main-loop aan te sturen.

		for (ui::events::event &e : event_source) {
			// event heeft een methode handle_quit die controleert
			// of de gebruiker de applicatie wilt sluiten, en zo ja
			// de meegegeven functie (of lambda) aanroept om met het
			// bijbehorende quit_event
			//
			e.handle_quit([&ctl](ui::events::quit_event qe) { ctl.quit = true; });
		}
	});
}
