#include "list_monitor.h"

QueueListMonitor::QueueListMonitor() {}

void QueueListMonitor::add_element(Queue<Snapshot>* queue, int player_id) {
    std::lock_guard<std::mutex> lock(m);
    list.emplace_back(queue, player_id);
}
Queue<Snapshot>* QueueListMonitor::remove_element(int player_id) {
    std::lock_guard<std::mutex> lock(m);

    Queue<Snapshot>* removed_queue = nullptr;

    list.remove_if([&removed_queue, player_id](const std::pair<Queue<Snapshot>*, int>& pair) {
        if (pair.second == player_id) {
            removed_queue = pair.first;
            return true;
        }
        return false;
    });

    return removed_queue;
}
void QueueListMonitor::send_to_every(const Snapshot& status) {
    std::lock_guard<std::mutex> lock(m);
    for (auto& pair: list) {
        try {
            pair.first->try_push(status);
        } catch (const ClosedQueue& e) {
            remove_element(pair.second);
        }
    }
}

QueueListMonitor::~QueueListMonitor() {}
