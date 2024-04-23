#pragma once

#include <string>
#include <vector>

enum class TokenType {
    exit,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

class Tokenizer {
public:
    inline explicit Tokenizer(std::string src):
        m_src(std::move(src))
    {

    }

    inline std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::string buffer;
        
        while(peek().has_value()) {
            if (std::isalpha(peek().value())) {
                buffer.push_back(consume());
                while (peek().has_value() && std::isalnum(peek().value())) {
                    buffer.push_back(consume());
                }

                if (buffer == "exit") {
                    tokens.push_back({.type = TokenType::exit});
                    buffer.clear();
                    continue;
                } else {
                    std::cerr << "Unknown token: " << buffer << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else if (std::isdigit(peek().value())) {
                buffer.push_back(consume());

                while (peek().has_value() && std::isdigit(peek().value())) {
                    buffer.push_back(consume());
                }

                tokens.push_back({.type = TokenType::int_lit, .value = buffer});
                buffer.clear();
                continue;

            } else if (peek().value() == ';') {
                consume();
                tokens.push_back({.type = TokenType::semi});
                continue;

            } else if (std::isspace(peek().value())) {
                consume();
                continue;
            } else {
                std::cerr << "You messed up! :)" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        m_index = 0;
        return tokens;
    }
    private:
        const std::string m_src;
        size_t m_index = 0;

        [[nodiscard]] inline std::optional<char> peek(int ahead = 1) const {
            if (m_index + ahead > m_src.length()) {
                return {};
            } else {
                return m_src.at(m_index);
            }
        }

        inline char consume() {
            return m_src.at(m_index++);
        }
};