#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <algorithm>
#include <map>

namespace kmint {
namespace pigisland {

map::map_node const &random_adjacent_node(map::map_node const &node) {
  int r = random_int(0, node.num_edges());
  return node[r].to();
}

map::map_node const &find_node_of_kind(map::map_graph const &graph, char kind) {
  auto i = std::find_if(graph.begin(), graph.end(), [kind](auto const &node) {
    return node.node_info().kind == kind;
  });
  return *i;
}

map::map_node const &find_random_mooring_place(map::map_graph const &graph) {
  return find_node_of_kind(graph, '1' + random_int(0, 3));
}

map::map_node const &find_shark_resting_place(map::map_graph const &graph) {
  return find_node_of_kind(graph, 'K');
}

const map::map_node* find_smallest_distance(std::map<const map::map_node*, float>& distances, std::vector<const map::map_node*>& closed_list, map::map_node const &end)
{
	const map::map_node* smallest_distance = nullptr;

	for (auto& pair : distances)
	{
		const auto heuristic_n = abs((pair.first->location().x() - end.location().x()) / 32) + abs((pair.first->location().y() - end.location().y()) / 32);
		const auto heuristic_s = smallest_distance ? abs((smallest_distance->location().x() - end.location().x()) / 32) + abs((smallest_distance->location().y() - end.location().y()) / 32) : 0;

		if (std::find(closed_list.begin(), closed_list.end(), pair.first) == closed_list.end() && 
			(smallest_distance == nullptr || pair.second + heuristic_n < distances.find(smallest_distance)->second + heuristic_s))
			smallest_distance = pair.first;
	}

	return smallest_distance;
}

std::unique_ptr<std::vector<const map::map_node*>> find_shortest_route(map::map_node const &current, map::map_node const &end)
{
	std::vector<const map::map_node*> open_list;
	std::vector<const map::map_node*> closed_list;
	std::map<const map::map_node*, float> distances;
	std::map<const map::map_node*, const map::map_node*> previous_node;

	open_list.emplace_back(&current);
	distances.insert(std::make_pair(&current, 0));
	previous_node.insert(std::make_pair(&current, nullptr));

	while (!open_list.empty())
	{
		auto smallest_distance = find_smallest_distance(distances, closed_list, end);
		open_list.erase(std::remove(open_list.begin(), open_list.end(), smallest_distance), open_list.end());

		if (smallest_distance == &end)
			break;

		for (auto i = 0; i < int(smallest_distance->num_edges()); i++)
		{
			auto neighbor = &(*smallest_distance)[i].to();
			
			if (std::find(closed_list.begin(), closed_list.end(), neighbor) == closed_list.end())
			{
				open_list.emplace_back(neighbor);
				previous_node.insert(std::make_pair(neighbor, smallest_distance));

				auto cost_n = distances.find(smallest_distance)->second + (*smallest_distance)[i].weight();
				const auto heuristic_n = abs((neighbor->location().x() - end.location().x()) / 32) + abs((neighbor->location().y() - end.location().y()) / 32);

				if (distances.find(neighbor) == distances.end())
					distances.insert(std::make_pair(neighbor, cost_n));
				else if (cost_n + heuristic_n < distances.find(neighbor)->second + heuristic_n)
				{
					distances[neighbor] = cost_n;
					previous_node[neighbor] = smallest_distance;
				}
			}
		}

		closed_list.push_back(smallest_distance);
		auto temp = const_cast<map::map_node*>(smallest_distance);
		temp->tagged(true);
	}

	auto route = std::make_unique<std::vector<const map::map_node*>>();
	auto temp_node = &end;

	while (temp_node != &current)
	{
		route->insert(route->begin(), temp_node);
		temp_node = previous_node.find(temp_node)->second;
	}

	return route;
}

int waiting_time(map::map_node const &node) {
  return static_cast<int>(node[0].weight());
}

} // namespace pigisland
} // namespace kmint
