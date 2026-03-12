# Main entry point for the AI Advisor

import random
import re

def suggest_modifications(speed, altitude, distance):
    print("Analyzing scramjet design...")
    # Placeholder AI logic for suggesting modifications
    modifications = [
        "Increase material thickness at the leading edge.",
        "Optimize cooling channels for high heat flux.",
        "Redesign inlet geometry for better shockwave control.",
    ]
    print("Suggested modifications:")
    for mod in modifications:
        print(f"- {mod}")

def chatbot_response(user_input):
    if re.search(r"speed|altitude|distance", user_input, re.IGNORECASE):
        return ("Based on the input parameters, I suggest:\n"
                "- Increase material thickness at the leading edge.\n"
                "- Optimize cooling channels for high heat flux.\n"
                "- Redesign inlet geometry for better shockwave control.")
    elif re.search(r"hello|hi", user_input, re.IGNORECASE):
        return "Hello! How can I assist you with your scramjet design today?"
    elif re.search(r"help", user_input, re.IGNORECASE):
        return ("Sure! You can ask me about speed, altitude, distance, or scramjet design improvements.")
    else:
        return "I'm sorry, I didn't understand that. Could you rephrase?"

def main():
    print("AI Advisor Initialized")
    print("Type 'exit' to quit.\n")

    while True:
        user_input = input("You: ")
        if user_input.lower() == "exit":
            print("AI: Goodbye!")
            break

        response = chatbot_response(user_input)
        print(f"AI: {response}")

if __name__ == "__main__":
    main()