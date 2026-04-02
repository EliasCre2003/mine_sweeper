                                                                                                                               #include "ui.hpp"

void renderGameGrid(GameGrid& gameGrid)
{
    unsigned int rows = gameGrid.numRows();
    unsigned int cols = gameGrid.numCols();
    ImVec2 cellSize{24, 24};

    std::map<CellCoord, unsigned int> revealed = gameGrid.revealedCells();

    if (ImGui::BeginTable("grid", cols, ImGuiTableFlags_SizingFixedFit))
    {
        for (unsigned int r = 0; r < rows; r++)
        {
            ImGui::TableNextRow();
            for (unsigned int c = 0; c < cols; c++)
            {
                CellCoord coord = {c, r};
                ImGui::TableSetColumnIndex(c);
                ImGui::PushID(r * rows + c);

                if (revealed.contains(coord))
                {
                    unsigned int n = revealed.at(coord);
                    if (n > 0)
                        ImGui::Text("%d", n);
                }
                else
                {
                    if (ImGui::Button("", cellSize))
                    {
                        gameGrid.clickCell(coord);
                        printf("asas");
                        // int n = gameGrid.revealed.size();
                        // printf("%d", n);
                    }
                }
                ImGui::PopID();
            }
        }
        ImGui::EndTable();
    }
}