#include "list_monitor.h"

QueueListMonitor::QueueListMonitor() {}

void QueueListMonitor::add_element(Queue<Snapshot>* queue, int player_id) {
    std::lock_guard<std::mutex> lock(m);
    list.emplace_back(queue, player_id);
}
void QueueListMonitor::remove_element(int player_id) {
    std::lock_guard<std::mutex> lock(m);
    list.remove_if([player_id](const std::pair<Queue<Snapshot>*, int>& pair) {
        bool data = pair.second == player_id;
        printf("eliminando queue en monitor por player id bool %d\n", data);
        return pair.second == player_id;
    });
}
void QueueListMonitor::send_to_every(const Snapshot& status) {
    std::lock_guard<std::mutex> lock(m);
    for (auto& pair: list) {
        pair.first->try_push(status);
    }
}

QueueListMonitor::~QueueListMonitor() {}
