#include <qpl/qpl.hpp>

constexpr auto key_threshold = std::array{ 3,7,15,5,6,11,3,16,12,19,4,7,13,6,7,2,5,11,12,5,5,12,4,3,16,8,15,9,6,7,15,15,6,4,2,9,5,5 };
constexpr auto key_importance = std::array{ 0,0,2,0,0,0,0,1,2,1,1,1,0,1,1,0,0,1,1,2,2,1,2,1,1,2,1,2,1,2,0,0,0,0,0,0,0,2 };
constexpr auto key_end = std::array{ 2,2,7,3,4,5,2,1,4,0,3,4,8,3,3,2,2,6,6,2,3,20,2,1,10,4,4,3,3,2,0,0,2,3,1,12,3,2 };

constexpr auto key_descriptions = std::array{
    "[dummy player in most cases] max out of any twenty inputs in a row where cursor is perfectly centered (+- 2/28 tee units) where at least one tee is moving and neither are on the same x/y plane",
    "[dummy player in most cases] max out of any twenty inputs in a row where cursor is perfectly centered (+- 5/28 tee units) where at least one tee is moving and neither are on the same x/y plane",
    "max out of any forty inputs in a row where the aim angle is centered on another tee, where one is moving and neither are on the same x/y plane",
    "[dummy player in most cases] max out of any 600 inputs in a row where the cursor position snapped from a far distance to near the center of a tee in one input",
    "[dummy player in most cases]",
    "[dummy player in most cases]",
    "[dummy player in most cases]",
    "[more avanced] angle based eyefollow systems that are more sensitive, but sometimes less reliable",
    "[more avanced] angle based eyefollow systems that are more sensitive, but sometimes less reliable. Much better indicate than H and J keys",
    "[more avanced] angle based eyefollow systems that are more sensitive, but sometimes less reliable",
    "detects a variety of more avanced hook aim bots that do not aim center",
    "detects the max out of any 30 consecutive hooks that were made after or during a harsh angle snap",
    "detects fast mouse movement during hooking",
    "detects the max out of any 30 consecutive hooks that were made with a weird distance/angle ratio",
    "detects the max out of any 30 consecutive hooks that were made after or during a harsh mouse position snap",
    "[dummy player in most cases]",
    "[dummy player in most cases]",
    "[can be false positive] detects simple hook aim bots that aim near center of the tee",
    "[can be false positive] detects simple hook aim bots that aim center angle of the tee",
    "detects a variety of hook assist bots when used in gameplay",
    "detects sharp mouse snapping during hooks",
    "detects hooking consistently and smoothly at center angle",
    "detects hook aim bots that snap near center of the tee",
    "detects more avanced hook aim bots that snap sometime before the hook is made",
    "detects hook aim bots that aim near the center of the tee",
    "detects hook assist bots when used in gameplay with quick sensitivity",
    "detects hook aim bots (can be triggered by nonstandard mouse distance)",
    "detects hook aim bots used in conjunction with spin bots or high mouse sensitivity",
    "detects hook assist bots",
    "detects hook assist bots such as new KRX or pwn client",
    "[can be false positive] detects hook assist on low range",
    "[can be false positive] detects hook assist on low range",
    "[dummy player in most cases]",
    "[dummy player in most cases]",
    "[dummy player in most cases]",
    "[dummy player in most cases]",
    "[dummy player in most cases]",
    "[beta] detects simple hook aim bots such as KRX that aim center angle, but not position of the tee",
};

constexpr auto key_colors = std::array{ qpl::foreground::white, qpl::foreground::yellow, qpl::foreground::light_red };

constexpr auto key_characters = std::array{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'
};
constexpr auto key_name = std::array{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm'
};

constexpr auto key_to_index(char c) {
    if (c >= 'A' && c <= 'Z') {
        return qpl::size_cast(c - 'A');
    }
    else {
        return qpl::size_cast(c - 'a' + 26);
    }
}

constexpr auto dump_position_to_key = std::array {
    0ull, 0ull, 1ull, 1ull, 2ull, 2ull, 3ull, 3ull, qpl::size_max, 4ull, 4ull, 5ull, 5ull, 6ull, 6ull, 7ull, 7ull, 8ull, 8ull, 9ull, 9ull, qpl::size_max, 10ull, 10ull, 11ull, 11ull, 12ull, 12ull, 13ull, 13ull, 14ull, 14ull, 15ull, 15ull, 16ull, 16ull, 17ull, 17ull, 18ull, 18ull, qpl::size_max, 19ull, 19ull, 20ull, 20ull, 21ull, 21ull, 22ull, 22ull, 23ull, 23ull, 24ull, 24ull, 25ull, 25ull, 26ull, 26ull, 27ull, 27ull, 28ull, 28ull, 29ull, 29ull, 30ull, 30ull, 31ull, 31ull, qpl::size_max, 32ull, 32ull, 33ull, 33ull, 34ull, 34ull, 35ull, 35ull, 36ull, 36ull, qpl::size_max, 37ull, 37ull, 37ull, 37ull, 37ull, 37ull, 37ull
};

namespace data {
    bool show_full_description = false;
}

struct antibot_parser {
    std::array<qpl::u8, 38> keys;
    std::wstring line;
    qpl::size numbers_start = 0u;

    std::array<qpl::size, 3u> importance_triggers;
    std::array<qpl::size, 3u> over_threshold_triggers;

    bool empty = true;
    bool any_active_key = false;

    antibot_parser() {
        this->reset();
    }
    void reset() {
        this->keys.fill(0.0);
        this->any_active_key = false;
        this->empty = false;
        this->numbers_start = 0u;
        this->importance_triggers.fill(0u);
        this->over_threshold_triggers.fill(0u);
        this->line = L"";
    }

    void print_antibot_dump_line(const std::wstring& string, const std::unordered_set<qpl::size>& active_keys) const {
        qpl::print(string.substr(0u, this->numbers_start));
        for (qpl::size i = this->numbers_start; i < string.length(); ++i) {
            if (i - this->numbers_start >= dump_position_to_key.size()) {
                qpl::print(string.substr(i));
                break;
            }
            auto key = dump_position_to_key.at(i - this->numbers_start);

            qpl::foreground color = qpl::foreground::gray;
            if (active_keys.find(key) != active_keys.cend()) {
                if (key != qpl::size_max) {
                    color = key_colors.at(key_importance.at(key));
                }
            }
            qpl::print(color, string.at(i));
        }
        qpl::println();
    }

    void print_info() const {

        std::unordered_set<qpl::size> active_keys;
        for (qpl::size i = 0u; i < this->keys.size(); ++i) {
            if (keys[i] >= key_threshold[i]) {
                active_keys.insert(i);
            }
        }
        //hooks key
        active_keys.insert(37u);

        auto overlay_line = qpl::to_wstring(qpl::to_string_repeat(" ", this->numbers_start - 4u), L"#ID  A B C D| E F G H I J| K L M N O P Q R S| T U V W X Y Z a b c d e f| g h i j k| l");

        this->print_antibot_dump_line(overlay_line, active_keys);
        this->print_antibot_dump_line(this->line, active_keys);
    }

    auto& get_key(char value) {
        return this->keys.at(key_to_index(value));
    }
    const auto& get_key(char value) const {
        return this->keys.at(key_to_index(value));
    }

    void print_result() const {
        if (this->empty || !this->any_active_key) {
            return;
        }

        this->print_info();
        qpl::println(qpl::to_string_repeat("-", this->line.length()));

        for (qpl::size i = 0u; i < this->keys.size(); ++i) {
            if (keys[i] >= key_threshold[i]) {
                qpl::size over = 0u;

                auto sub = qpl::signed_cast(this->keys[i]) - key_threshold[i];
                if (sub > key_end[i]) {
                    over = sub - key_end[i];
                }
                auto exclamations = qpl::min(over / 5, qpl::size{ 3 });

                auto color = key_colors[key_importance[i]];
                auto percentage = this->keys[i];

                std::string description = key_descriptions[i];
                if (!data::show_full_description && description.length() > 120) {
                    description.resize(117);
                    description += "...";
                }

                qpl::println("  ", 
                    color, qpl::to_string("key ", key_name[i], ": ", qpl::str_spaced(qpl::hex_string(this->keys[i], "", qpl::base_format::base36u), 2)),
                    " ", qpl::foreground::light_red, qpl::str_spaced(over ? qpl::to_string("(+", qpl::size_cast(over), qpl::to_string_repeat("!", exclamations), ")") : "", 8),
                    " ", qpl::foreground::gray, description);
            }
        }
        qpl::println("\n\n");
    }

    void calculate_threshold_arrays() {
        for (qpl::size i = 0u; i< this->keys.size(); ++i) {
            if (this->keys[i] >= key_threshold[i]) {
                this->any_active_key = true;
                ++this->importance_triggers[key_importance[i]];

                auto sub = qpl::signed_cast(this->keys[i]) - key_threshold[i];
                if (sub > 0) {
                    this->over_threshold_triggers[key_importance[i]] += sub;
                }
            }
        }
    }
    void calculate_additional_keys() {
        //calculate 'm' key

        auto r = this->get_key('R');
        auto s = this->get_key('S');

        qpl::u8 q;
        if (r == 0) {
            q = s + 1;
        }
        else {
            q = qpl::u8_cast(std::round(s / qpl::f64_cast(r)));
        }
        this->get_key('l') = q;

    }
    void parse(std::wstring line) {
        this->reset();

        auto str_line = qpl::wstring_to_string(line);
        this->empty = !qpl::string_contains(str_line, "antibot");
        if (this->empty) {
            return;
        }
        this->line = line;
        this->numbers_start = qpl::string_find(str_line, "|") - 8;

        line = qpl::string_remove_all(line, L"|");

        qpl::size ctr = 0u;
        for (qpl::size i = this->numbers_start; i < this->numbers_start + 37 * 2; i += 2) {
            auto value = qpl::from_base_string(qpl::string_remove_whitespace(qpl::wstring_to_string(line.substr(i, 2u))), 16);
            this->keys.at(ctr) = value;
            ++ctr;
        }

        this->calculate_additional_keys();
        this->calculate_threshold_arrays();
    }
    bool operator<(const antibot_parser& other) const {
        for (qpl::isize i = this->importance_triggers.size() - 1; i >= 0; --i) {
            if (this->importance_triggers[i] < other.importance_triggers[i]) {
                return true;
            }
            else if (this->importance_triggers[i] > other.importance_triggers[i]) {
                return false;
            }
            else if (this->over_threshold_triggers[i] < other.over_threshold_triggers[i]) {
                return true;
            }
            else if (this->over_threshold_triggers[i] > other.over_threshold_triggers[i]) {
                return false;
            }
        }
        return false;
    }
};

int main() {
    qpl::winsys::enable_utf();

    qpl::println("(C)ReD (ReD#7561, https://github.com/DanielRabl)\n");
    qpl::println("parses rcon antibot_dump into human-readable form.");
    qpl::println("\"full description 1\" and \"full description 0\" to toggle long description cutoff (0 by default).");
    qpl::println(qpl::foreground::white,   "white key:   ", "LOW  importance.");
    qpl::println(qpl::foreground::yellow,  "yellow key:  ", "MID  importance.");
    qpl::println(qpl::foreground::light_red, "red key:     ", "HIGH importance.\n");

    for (qpl::size i = 0u; i < key_descriptions.size(); ++i) {
        qpl::println(
            key_colors[key_importance[i]], qpl::str_spaced(qpl::to_string("key ", key_name[i], " (", qpl::base_string(key_threshold[i], 16, ""), " - ", qpl::base_string(key_end[i], 16, ""), ")"), 15),
            ": ",
            qpl::foreground::gray, key_descriptions[i]);
    }
    qpl::println();
    qpl::println();

    bool show_full_description = false;

    while (true) {
        try {
            qpl::println("enter antibot dump: ");

            auto input = qpl::get_multiline_input_wstring();
            auto input_lines = qpl::string_split(input, L'\n');

            if (input_lines.size() == 1u) {
                if (input_lines[0] == L"full description 0") {
                    qpl::println(qpl::foreground::aqua, "set full description to 0.");
                    data::show_full_description = false;
                    continue;
                }
                if (input_lines[0] == L"full description 1") {
                    qpl::println(qpl::foreground::aqua, "set full description to 1.");
                    data::show_full_description = true;
                    continue;
                }
            }

            std::vector<antibot_parser> antibot_input(input_lines.size());

            std::vector<antibot_parser> antibot;
            for (qpl::size i = 0u; i < input_lines.size(); ++i) {
                antibot_input[i].parse(input_lines[i]);
                if (!antibot_input[i].empty) {
                    antibot.push_back(antibot_input[i]);
                }
            }
            qpl::sort(antibot);

            qpl::size active_count = 0u;
            qpl::println('\n');
            for (auto& i : antibot) {
                if (i.any_active_key) {
                    ++active_count;
                }
                i.print_result();
            }


            auto inactive = antibot.size() - active_count;
            if (antibot.size()) {
                std::string result = "";
                if (!active_count) {
                    if (antibot.size() == 1) {
                        antibot[0].print_info();
                        qpl::println();
                        result = qpl::to_string("This line has no suspicious values.");
                    }
                    else {
                        result = qpl::to_string("None of the ", antibot.size(), " lines have suspicious values.");
                    }
                }
                else {
                    if (antibot.size() == 1) {
                        result = qpl::to_string("This line has suspicious values.");
                    }
                    else if (!inactive) {
                        result = qpl::to_string("All ", antibot.size(), " lines have suspicious values.");
                    }
                    else {
                        result = qpl::to_string(active_count, " / ", antibot.size(), " lines have suspicious values.");
                    }
                }
                qpl::println(" +", qpl::to_string_repeat("-", result.length() + 2), '+');
                qpl::println(" | ", qpl::foreground::bright_white, result, " |");
                qpl::println(" +", qpl::to_string_repeat("-", result.length() + 2), "+\n");
            }
            else {
                qpl::println("ignored this input, no antibot lines detected.");
            }
        }
        catch (std::exception& any) {
            qpl::println(qpl::foreground::light_red, "caught exception:\n", any.what(), '\n');
        }
    }
}