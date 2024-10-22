#ifndef QUEUE_LIST_MONITOR_H
#define QUEUE_LIST_MONITOR_H

#include <list>
#include <mutex>

#include "common/blocking_queue.h"
#include "common/snapshot.h"

class QueueListMonitor {
private:
    std::mutex m;
    std::list<Queue<struct Snapshot>*> status_queue_list;

public:
    QueueListMonitor();
    // Append a new Queue reference to the list.
    void add_element(Queue<struct Snapshot>* queue);
    // Remove a Queue reference from the list.
    void remove_element(Queue<struct Snapshot>* queue);
    // push the string recieved to all the queues.
    /*  Closed queues should not be in this monitor.
     *   If some of the queues in it is closed it will throw ClosedQueue exception.
     */
    void send_to_every(const struct Snapshot&);
    ~QueueListMonitor();

private:
    QueueListMonitor(const QueueListMonitor&) = delete;
    QueueListMonitor& operator=(const QueueListMonitor&) = delete;
};


#endif