#ifndef GAMEJUDGE_H
#define GAMEJUDGE_H


template <typename data_t>
class game_judge
{
public:
    const data_t &_data;
    game_judge(const data_t &data): _data(data) {

    }

    enum result_t {
        white_win = -1,
        black_win = 1,
        unknown = 0,
        draw = 2
    };

    result_t judge() {
        for (int i = 0; i < data.get_row_count(); ++i) {
            get
        }
        for (int j = 0; j < data.get_column_count(); ++j) {

        }
        for (int i = 0; i < data.get_diag_count(); ++i) {

        }
        for (int i = 0; i < data.get_counter_diag_count(); ++i) {

        }
    }
};

#endif // GAMEJUDGE_H
