#include "kmint/main.hpp" // voor de main loop
#include "kmint/pigisland/boat.hpp"
#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/pigisland/shark.hpp"
#include "kmint/play.hpp"
#include "kmint/ui.hpp"

using namespace kmint;
int main() {
  // een app object is nodig om
  ui::app app{};

  //  maak een venster aan
  ui::window window{app.create_window({1024, 768}, "hello")};

  // maak een podium aan
  play::stage s{};

  auto map = pigisland::map();
  map.graph()[0].tagged(true);
  s.build_actor<play::background>(math::size(1024, 768),
                                  graphics::image{map.background_image()});
  s.build_actor<play::map_actor>(math::vector2d{0.f, 0.f}, map.graph());
  
  auto& shark = s.build_actor<pigisland::shark>(map.graph());
  auto& boat = s.build_actor<pigisland::boat>(map.graph());

  for (int i = 0; i < 100; ++i) 
  {
	auto& pig = s.build_actor<pigisland::pig>(math::vector2d(i * 10.0f, i * 6.0f));
	pig.set_boat(boat);
	pig.set_shark(shark);
	shark.add_pig(pig);
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
