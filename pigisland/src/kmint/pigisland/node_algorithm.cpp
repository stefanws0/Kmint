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

const map::map_node* find_smallest_distance(std::map<const map::map_node*, float>& distances, std::vector<const map::map_node*>& closedList, map::map_node const &end)
{
	const map::map_node* smallestDistance = nullptr;

	for (auto& pair : distances)
	{
		float heuristic_n = abs((pair.first->location().x() - end.location().x()) / 32) + abs((pair.first->location().y() - end.location().y()) / 32);
		float heuristic_s = smallestDistance ? abs((smallestDistance->location().x() - end.location().x()) / 32) + abs((smallestDistance->location().y() - end.location().y()) / 32) : 0;

		if (std::find(closedList.begin(), closedList.end(), pair.first) == closedList.end() && 
			(smallestDistance == nullptr || pair.second + heuristic_n < distances.find(smallestDistance)->second + heuristic_s))
			smallestDistance = pair.first;
	}

	return smallestDistance;
}

std::unique_ptr<std::vector<const map::map_node*>> find_shortest_route(map::map_node const &current, map::map_node const &end)
{
	std::vector<const map::map_node*> openList;
	std::vector<const map::map_node*> closedList;
	std::map<const map::map_node*, float> distances;
	std::map<const map::map_node*, const map::map_node*> previousNode;

	openList.emplace_back(&current);
	distances.insert(std::make_pair(&current, 0));
	previousNode.insert(std::make_pair(&current, nullptr));

	while (!openList.empty())
	{
		const map::map_node* smallestDistance = find_smallest_distance(distances, closedList, end);
		openList.erase(std::remove(openList.begin(), openList.end(), smallestDistance), openList.end());

		if (smallestDistance == &end)
			break;

		for (int i = 0; i < smallestDistance->num_edges(); i++)
		{
			const map::map_node* neighbor = &(*smallestDistance)[i].to();
			
			if (std::find(closedList.begin(), closedList.end(), neighbor) == closedList.end())
			{
				openList.emplace_back(neighbor);
				previousNode.insert(std::make_pair(neighbor, smallestDistance));
				
				float cost_n = distances.find(smallestDistance)->second + (*smallestDistance)[i].weight();
				float heuristic_n = abs((neighbor->location().x() - end.location().x()) / 32) + abs((neighbor->location().y() - end.location().y()) / 32);

				if (distances.find(neighbor) == distances.end())
					distances.insert(std::make_pair(neighbor, cost_n));
				else if (cost_n + heuristic_n < distances.find(neighbor)->second + heuristic_n)
				{
					distances[neighbor] = cost_n;
					previousNode[neighbor] = smallestDistance;
				}
			}
		}

		closedList.push_back(smallestDistance);
		map::map_node* temp = const_cast<map::map_node*>(smallestDistance);
		temp->tagged(true);
	}

	auto route = std::make_unique<std::vector<const map::map_node*>>();
	const map::map_node* tempNode = &end;

	while (tempNode != &current)
	{
		route->insert(route->begin(), tempNode);
		tempNode = previousNode.find(tempNode)->second;
	}

	return std::move(route);
}

int waiting_time(map::map_node const &node) {
  return static_cast<int>(node[0].weight());
}

} // namespace pigisland
} // namespace kmint
