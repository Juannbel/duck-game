#include "list_monitor.h"

QueueListMonitor::QueueListMonitor() {}

void QueueListMonitor::add_element(Queue<std::string>* queue) {
    std::lock_guard<std::mutex> lck(m);
    msj_queue_list.push_back(queue);
}
void QueueListMonitor::remove_element(Queue<std::string>* queue) {
    std::lock_guard<std::mutex> lck(m);
    msj_queue_list.remove(queue);
}
void QueueListMonitor::send_to_every(const std::string& msj) {
    std::lock_guard<std::mutex> lck(m);
    for (auto* queue: msj_queue_list) {
        queue->try_push(msj);
    }
}
QueueListMonitor::~QueueListMonitor() {}
