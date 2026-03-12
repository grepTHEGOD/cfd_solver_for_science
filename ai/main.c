#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void suggestModifications(double speed, double altitude, double distance) {
    printf("Analyzing scramjet design...\n");
    // Placeholder AI logic for suggesting modifications
    printf("Suggested modifications:\n");
    printf("- Increase material thickness at the leading edge.\n");
    printf("- Optimize cooling channels for high heat flux.\n");
    printf("- Redesign inlet geometry for better shockwave control.\n");
}

void chatbotResponse(const char *input) {
    if (strstr(input, "speed") || strstr(input, "altitude") || strstr(input, "distance")) {
        printf("AI: Based on the input parameters, I suggest:\n");
        printf("- Increase material thickness at the leading edge.\n");
        printf("- Optimize cooling channels for high heat flux.\n");
        printf("- Redesign inlet geometry for better shockwave control.\n");
    } else if (strstr(input, "hello") || strstr(input, "hi")) {
        printf("AI: Hello! How can I assist you with your scramjet design today?\n");
    } else if (strstr(input, "help")) {
        printf("AI: Sure! You can ask me about speed, altitude, distance, or scramjet design improvements.\n");
    } else {
        printf("AI: I'm sorry, I didn't understand that. Could you rephrase?\n");
    }
}

void runPythonChatbot() {
    printf("Launching Python Chatbot...\n");
    int result = system("python3 /home/grep/cfd_solver_for_science/ai/main.py");
    if (result != 0) {
        printf("Failed to launch Python Chatbot.\n");
    }
}

int main() {
    double speed, altitude, distance;
    char input[256];
    printf("AI Advisor Initialized\n");
    printf("Enter speed (m/s): ");
    scanf("%lf", &speed);
    printf("Enter altitude (m): ");
    scanf("%lf", &altitude);
    printf("Enter distance (km): ");
    scanf("%lf", &distance);

    suggestModifications(speed, altitude, distance);

    printf("AI Chatbot Initialized\n");
    printf("Type 'exit' to quit.\n\n");

    while (1) {
        printf("You: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Remove newline character

        if (strcmp(input, "exit") == 0) {
            printf("AI: Goodbye!\n");
            break;
        }

        chatbotResponse(input);
    }

    runPythonChatbot();

    return 0;
}