#include "list_monitor.h"

QueueListMonitor::QueueListMonitor() {}

void QueueListMonitor::add_element(Queue<struct Snapshot>* queue) {
    std::lock_guard<std::mutex> lck(m);
    status_queue_list.push_back(queue);
}
void QueueListMonitor::remove_element(Queue<struct Snapshot>* queue) {
    std::lock_guard<std::mutex> lck(m);
    status_queue_list.remove(queue);
}
void QueueListMonitor::send_to_every(const struct Snapshot& status) {
    std::lock_guard<std::mutex> lck(m);
    for (auto* queue: status_queue_list) {
        queue->try_push(status);
    }
}
QueueListMonitor::~QueueListMonitor() {}
