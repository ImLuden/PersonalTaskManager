#include "task_manager.h"

// Converts a TaskStatus enum value to its corresponding string representation.
std::string Task::task_status_to_string(TaskStatus status)

{
    switch (status)
    {
    case TaskStatus::Pending:
        return "Pending";
    case TaskStatus::Completed:
        return "Completed";
    case TaskStatus::Expired:
        return "Expired";
    default:
        return "Unknown"; // Fallback for unexpected values
    }
}

// Method to convert a string to TaskStatus using unordered_map
TaskStatus Task::stringToTaskStatus(const std::string& statusStr)
{

    static const std::unordered_map<std::string, TaskStatus> statusMap =
    {
        {"pending", TaskStatus::Pending},
        {"completed", TaskStatus::Completed},
        {"expired", TaskStatus::Expired}
    };

    std::string lowerStatus = toLowerAndTrim(statusStr);

    auto it = statusMap.find(lowerStatus);
    if (it != statusMap.end())
    {
        return it->second; // Return the corresponding TaskStatus
    }

    throw std::invalid_argument("Invalid task status string: " + statusStr);
}

nlohmann::json Task::to_json() const
{
    return
    {
        {"id", id},
        {"title", title},
        {"description", description},
        {"due_date", due_date},
        {"status", task_status_to_string(status)}
    };
}

// Converts JSON data back to Task object with all necessary fields.
// This static method is used to load tasks from JSON file format.
Task Task::from_json(const nlohmann::json& j)
{
    Task task;

    task.id = j.at("id").get<int>();
    task.title = j.at("title").get<std::string>();
    task.description = j.at("description").get<std::string>();
    task.due_date = j.at("due_date").get<std::string>();

    // Convert the "status" field based on its type
    if (j.contains("status")) {
        if (j.at("status").is_string()) {
            task.status = stringToTaskStatus(j.at("status").get<std::string>());
        }
        else if (j.at("status").is_number_integer()) {
            // Handle numerical status
            task.status = static_cast<TaskStatus>(j.at("status").get<int>());
        }
        else {
            throw std::invalid_argument("Invalid type for 'status' field.");
        }
    }

    return task;
}