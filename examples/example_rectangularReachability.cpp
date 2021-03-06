/**
 * @brief Example file which shows how to instanciate a concrete worker out of context handlers.
 */

#include "representations/GeometricObject.h"
#include <datastructures/reachability/SettingsProvider.h>
#include <datastructures/reachability/ReachTree.h>
#include <datastructures/reachability/Task.h>
#include <datastructures/reachability/workQueue/WorkQueueManager.h>
#include <datastructures/reachability/timing/EventTimingProvider.h>
#include <algorithms/reachability/workers/ContextBasedReachabilityWorker.h>
#include <iostream>

int main() {

    // typedefs
    using Number = double;
    using State = hypro::State_t<Number>;

    // settings provider instance as reference for readability
    hypro::SettingsProvider<State>& settingsProvider = hypro::SettingsProvider<State>::getInstance();

    // variables
    carl::Variable x = hypro::VariablePool::getInstance().carlVarByIndex(0);
    carl::Variable y = hypro::VariablePool::getInstance().carlVarByIndex(1);
    // rectangular dynamics
    std::map<carl::Variable, carl::Interval<Number>> dynamics;
    dynamics.emplace(std::make_pair(x,carl::Interval<Number>(1,2)));
    dynamics.emplace(std::make_pair(y,carl::Interval<Number>(2,3)));

    // hybrid automaton instance
    hypro::HybridAutomaton<Number> ha;

    // create one location with rectangular flow
    hypro::Location<Number> loc1;
    hypro::rectangularFlow<Number> flow(dynamics);
    loc1.setRectangularFlow(flow);

    // add location
    ha.addLocation(loc1);

    // initial set is a unit box
    hypro::matrix_t<Number> constraints = hypro::matrix_t<Number>(4,2);
    constraints << 1,0,-1,0,0,1,0,-1;
    hypro::vector_t<Number> constants = hypro::vector_t<Number>(4);
    constants << 1,1,1,1;

    // create and add initial state
    ha.addInitialState(&loc1, hypro::Condition<Number>(constraints,constants));

    // theoretically we do not need this - check if really needed.
    settingsProvider.addStrategyElement<hypro::CarlPolytope<Number>>(mpq_class(1)/mpq_class(100), hypro::AGG_SETTING::AGG, -1);

    // set settings
    settingsProvider.setHybridAutomaton(std::move(ha));
    hypro::ReachabilitySettings settings;
    settings.useInvariantTimingInformation = false;
    settings.useGuardTimingInformation = false;
    settings.useBadStateTimingInformation = false;
    settingsProvider.setReachabilitySettings(settings);

    // set up queues
    hypro::WorkQueueManager<std::shared_ptr<hypro::Task<State>>> queueManager;
    auto globalCEXQueue = queueManager.addQueue();
    auto globalQueue = queueManager.addQueue();

    // set up initial states to the correct representation - in this case CarlPolytope
    auto initialStates = settingsProvider.getHybridAutomaton().getInitialStates();
    std::vector<hypro::State_t<Number>> initialStateData;
    for (auto stateMapIt = initialStates.begin(); stateMapIt != initialStates.end(); ++stateMapIt) {

        hypro::State_t<Number> copyState;
        copyState.setLocation(stateMapIt->first);
        copyState.setTimestamp(carl::Interval<hypro::tNumber>(0));

        // if the decider is in use - convert subspaces according to mapping
        State::repVariant _temp;
        _temp = hypro::CarlPolytope<Number>(stateMapIt->second.getMatrix(), stateMapIt->second.getVector());

        copyState.setSet(_temp,0);
        initialStateData.emplace_back(copyState);
    }

    // set up tree
    hypro::ReachTree<State> tree = hypro::ReachTree<State>(new hypro::ReachTreeNode<State>());
    std::vector<hypro::ReachTreeNode<State>*> initialNodes;
    for(const auto& state : initialStateData) {
        hypro::ReachTreeNode<State>* n = new hypro::ReachTreeNode<State>(state,0,tree.getRoot());
        n->setTimestamp(0, carl::Interval<hypro::tNumber>(0));
        initialNodes.push_back(n);
        tree.getRoot()->addChild(n);
    }

    // fill task queue with initial states
    for (const auto& initialNode : initialNodes) {
        globalQueue->enqueue(std::shared_ptr<hypro::Task<State>>(new hypro::Task<State>(initialNode)));
    }

    // create worker
    hypro::ContextBasedReachabilityWorker<State> worker = hypro::ContextBasedReachabilityWorker<State>(settings);

    // data structure to store computed flowpipes
    hypro::Flowpipe<State> segments;

    while(queueManager.hasWorkable(true)) { // locking access to queues.
        auto task = queueManager.getNextWorkable(true,true); // get next task locked and dequeue from front.
        assert(task!=nullptr);
        worker.processTask(task,settingsProvider.getStrategy(), globalQueue, globalCEXQueue, segments);
    }

    for(const auto& segment : segments) {
        std::cout << segment << std::endl;
    }

    return 0;
}
