TEST_CASE("Check if Dijkstra's works on a single airport node") {
    vector<Airport> airportDijk = readAirports("data/airports.dat~");
    Graph dijkstra = Graph(airportDijk);
    output = dijkstra.shortestPath(airportDijk.front(), airportDijk.front());
    path = output.second();

    REQUIRE(dijkstra.airports_.size() == 1);
    REQUIRE(path.size() == 1);
}

TEST_CASE("Check if Dijkstra's works for direct flights") {
    vector<Airport> airports = readAirports("data/test-airports.dat");
    vector<Route> routes = readRoutes("data/test-routes.dat");
    Graph graph = Graph(airports);
    output = graph.shortestPath(airports.front(), airports.back());
    path = output.second();
    pathDist = output.first();

    REQUIRE(general.airports_.size() == 3);
    REQUIRE(pathDist == 124.48);
}

TEST_CASE("Check if Dijkstra's works for not direct flights") {
    vector<Airport> airports = readAirports("data/test1-airports.dat");
    vector<Route> routes = readRoutes("data/test1-routes.dat");
    Graph graph = Graph(airports);
    output = graph.shortestPath(airports.front(), airports.back());
    path = output.second();
    pathDist = output.first();

    REQUIRE(graph.airports_.size() == 6);
    REQUIRE(pathDist == 337.022) 
}

//traversal

TEST_CASE("Check if traversal is working") {
    vector<Airport> airports = readAirports("data/test1-airports.dat");
    vector<Route> routes = readRoutes("data/test1-airports.dat");
    Graph graph = Graph(airports);
    output = graph.DFT(airports.front());
    for (unsigned i = 0; i < airports.front().routes_.size(); i++) {
        REQUIRE(graph.visited_[i] == true);
    }
}

TEST_CASE("Check if traversal is working for unconnected components") {
    vector<Airport> airports = readAirports("data/airports-dft.dat");
    vector<Route> routes = readRoutes("data/routes-dft.dat");
    Graph graph = Graph(airports);
    output = graph.DFTDis();
    for (unsigned i = 0; i < airports.front().routes_.size(); i++) {
        REQUIRE(graph.visited_[i] == true);
    }
}