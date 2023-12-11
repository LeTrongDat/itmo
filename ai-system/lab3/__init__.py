import re
from pyswip import Prolog

def match_pattern(user_input):
    patterns = {
        "List all popular games available on PC.": "popular_game(Game)",
        "Show games released on multiple platforms.": "available_on_multiple_platforms(Game)",
        "Identify games that are categorized as RPG.": "is_rpg(Game)",
        "Which games belong to both the action and adventure genres?": "is_action_adventure(Game)",
        "I'm looking for (.+) games released on (.+).": "genre_on_platform(Game, {}, {})",
        "Find games available on both (.+) and (.+).": "released_on_both_platforms(Game, {}, {})"
    }

    for pattern, query in patterns.items():
        match = re.match(pattern, user_input)
        if match:
            return query, match.groups()

    return None, None

def query_prolog(query, params):
    prolog = Prolog()
    prolog.consult('base-knowledge.pl')  # Replace with your Prolog file path
    query_string = query.format(*params)
    return list(prolog.query(query_string))

def main():
    while True:
        user_input = input("Enter your query (or 'exit' to quit): ")
        if user_input.lower() == 'exit':
            break

        query, params = match_pattern(user_input)
        if query:
            results = query_prolog(query, params)
            print("Results:", results)
        else:
            print("No matching pattern found.")

if __name__ == "__main__":
    main()
