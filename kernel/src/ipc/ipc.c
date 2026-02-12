#include <stddef.h>
#include <kernel/ipc.h>
#include <kernel/mm/mm.h>
#include <kernel/printk.h>
#include <kernel/string.h>
#include <kernel/list.h>

typedef struct {
    uint32_t from_pid;
    uint32_t to_pid;
    uint32_t type;
    uint32_t size;
    uint8_t data[0];
} ipc_message_t;

// Структура, где list является первым полем - это позволяет избежать offsetof
struct ipc_message_node {
    struct list_head list;
    ipc_message_t message;
};

static struct list_head message_queues[32];

void ipc_init() {
    for (int i = 0; i < 32; i++) {
        INIT_LIST_HEAD(&message_queues[i]);
    }
    printk("IPC system initialized\n");
}

int ipc_send(uint32_t to_pid, void* data, uint32_t size, uint32_t type) {
    if (to_pid >= 32) return 0;

    // Выделяем память для сообщения
    struct ipc_message_node* node = kmalloc(sizeof(struct ipc_message_node) + size);
    if (!node) return 0;

    // Заполняем сообщение
    node->message.from_pid = get_current_pid();
    node->message.to_pid = to_pid;
    node->message.type = type;
    node->message.size = size;
    memcpy(node->message.data, data, size);

    // Инициализируем список
    INIT_LIST_HEAD(&node->list);

    // Добавляем в очередь получателя
    list_add_tail(&node->list, &message_queues[to_pid]);

    printk("IPC message sent from %d to %d, type %d, size %d\n",
           node->message.from_pid, to_pid, type, size);
    return 1;
}

int ipc_receive(uint32_t* from_pid, void* buffer, uint32_t size, uint32_t* type) {
    uint32_t pid = get_current_pid();
    if (pid >= 32) return 0;

    // Проверяем очередь сообщений
    if (list_empty(&message_queues[pid])) {
        return 0; // Очередь пуста
    }

    // Извлекаем первое сообщение
    struct list_head *first = message_queues[pid].next;

    // Поскольку list является первым полем, мы можем просто привести тип
    struct ipc_message_node* node = (struct ipc_message_node*)first;

    list_del(first);

    // Копируем данные
    if (from_pid) *from_pid = node->message.from_pid;
    if (type) *type = node->message.type;

    uint32_t copy_size = (size < node->message.size) ? size : node->message.size;
    if (copy_size > 0 && buffer) {
        memcpy(buffer, node->message.data, copy_size);
    }

    // Освобождаем память
    kfree(node);

    printk("IPC message received by %d from %d, type %d, size %d\n",
           pid, node->message.from_pid, node->message.type, copy_size);
    return copy_size;
}

void ipc_process_messages(void) {
    uint32_t current_pid = get_current_pid();

    // Обрабатываем только если у текущего процесса есть сообщения
    if (current_pid >= 32) return;

    // Проверяем очередь сообщений текущего процесса
    if (list_empty(&message_queues[current_pid])) {
        return; // Очередь пуста
    }

    struct list_head *pos;
    struct ipc_message_node* node;

    // Обрабатываем все сообщения в очереди
    list_for_each(pos, &message_queues[current_pid]) {
        // Поскольку list является первым полем, мы можем просто привести тип
        node = (struct ipc_message_node*)pos;

        // Обрабатываем сообщение в зависимости от типа
        switch (node->message.type) {
            case IPC_TYPE_SYSTEM:
                printk("System IPC message from PID %d: %s\n",
                       node->message.from_pid, node->message.data);
                break;

            case IPC_TYPE_DATA:
                printk("Data IPC message from PID %d, size: %d\n",
                       node->message.from_pid, node->message.size);
                break;

            case IPC_TYPE_COMMAND:
                printk("Command IPC message from PID %d: %d\n",
                       node->message.from_pid, *(int*)node->message.data);
                break;

            default:
                printk("Unknown IPC message type %d from PID %d\n",
                       node->message.type, node->message.from_pid);
                break;
        }

        // Удаляем сообщение из очереди
        list_del(pos);
        kfree(node);

        // Выходим после обработки одного сообщения, чтобы не обрабатывать всю очередь за раз
        break;
    }
}
