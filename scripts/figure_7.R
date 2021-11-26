# load necessary packages
if (!require("pacman")) install.packages("pacman")
pacman::p_load(dplyr, ggplot2, here, cowplot, plotly)

# relative path
data_relative_path <- "../data/out/out_fig7/"

# scenario simulation to be included in the Figure
simulations <- "fig7"

# names of the .csv files
names_files <- list.files(path = data_relative_path,
                          pattern = paste0(simulations,
                                           ".*\\.csv$"))

# read all the .csv files for the analysis
data_list <- lapply(paste0(data_relative_path, names_files), read.csv)

# names of the dataframes, i.e., remove extension file (.csv)
names_df <- gsub(".csv", "", names_files)

# Rename data frames within the list with the name files
names(data_list) <- names_df

# sum total of viable seeds per year per cohort type (i.e., inter-fire and post-fire)
df_past <- data_list[[1]] %>% 
  dplyr::group_by(pollination_mean, fire_mean) %>%
  dplyr::summarize(mean_persistence = mean(persistence_time, na.rm = TRUE),
                   median_persistence = median(persistence_time, na.rm = TRUE),
                   mean_generations = mean(num_generations, na.rm = TRUE),
                   median_generations = median(num_generations, na.rm = TRUE),
                   max_persistence = max(persistence_time, na.rm = TRUE))


# nrow refers to fire_mean from 10 years to 35 years in 1 year steps
# ncol refers to pollination_mean from 0% to 100% in 5% steps
matrix_past_mean <- matrix(df_past$mean_persistence,
                           nrow = 26, ncol = 21)

matrix_past_median <- matrix(df_past$median_persistence, 
                             nrow = 26, ncol = 21)

matrix_past_generations_mean <- matrix(df_past$mean_generations, 
                                       nrow = 26, ncol = 21)

matrix_past_generations_median <- matrix(df_past$median_generations, 
                                         nrow = 26, ncol = 21)



# sum total of viable seeds per year per cohort type (i.e., inter-fire and post-fire)
df_past_stoch <- data_list[[2]] %>% 
  dplyr::group_by(pollination_mean, fire_mean) %>%
  dplyr::summarize(mean_persistence = mean(persistence_time, na.rm = TRUE),
                   median_persistence = median(persistence_time, na.rm = TRUE),
                   mean_generations = mean(num_generations, na.rm = TRUE),
                   median_generations = median(num_generations, na.rm = TRUE),
                   max_persistence = max(persistence_time, na.rm = TRUE))


matrix_past_stoch_mean <- matrix(
  df_past_stoch$mean_persistence, nrow = 26, ncol = 21)

matrix_past_stoch_generations_mean <- matrix(
  df_past_stoch$mean_generations, nrow = 26, ncol = 21)

matrix_past_stoch_median <- matrix(
  df_past_stoch$median_persistence, nrow = 26, ncol = 21)

matrix_past_stoch_generations_median <- matrix(
  df_past_stoch$median_generations, nrow = 26, ncol = 21)


#-----------------------------------------------------------------------------

t <- list(
#  family = "mono",
  size = 16)

axx <- list(
  title = "<b>Pollination success (%)</b>",
  font = t,
  tickvals = c(0,25,50,75,100),
  # autotick = TRUE,
  nticks = 5,
  domain = c(0, 100),
  showgrid = T,
  tickmode = "array"
)

axy <- list(
  title = "<b>Fire interval (yr)</b>",
  font = t,
  zeroline = T,
  autotick = T,
  nticks = 10,
  start = 10,
  domain = c(10, 39),
  range = c(10, 39)
)

axz1 <- list(
  title = "<b>Persistence (yr)</b>",
  nticks = 6,
  domain = c(0, 500),
  range = c(0, 500)
)

axz2 <- list(
  title = "<b># of generations</b>",
  nticks = 5,
  domain = c(0, 45),
  range = c(0, 45)
)

camera <- list(eye = list(x = -1.25, y = -0.75, z = 1.85))


p7a_mean <- plot_ly(x = seq(0, 100, by = 5),
                    y = seq(10, 35, by = 1),
                    z = ~matrix_past_mean,
                    scene = "scene2",
                    showscale = FALSE) %>%
  add_surface() %>%
  layout(
    title = list(
      text = "<br><b>a)</b>",
      font = list(
        size = 26
        ),
      x    = 0,
      xref = "paper",
      yref = "paper"),
    margin = list(
      l = 0,
      r = 0,
      t = 30,
      b = 0
    ),
    font   = t,
    scene2 = list(xaxis = axx,
                  yaxis = axy,
                  zaxis = axz1,
                  camera = camera))


p7b_mean <- plot_ly(x = seq(0, 100, by = 5),
                    y = seq(10.0001, 35, by = 1),
                    z = ~matrix_past_generations_mean,
                    scene = "scene3",
                    showscale = FALSE) %>%
  add_surface() %>%
  layout(
    title = list(
      text = "<br><b>b)</b>",
      font = list(
        size = 26
      ),
      x    = 0,
      xref = "paper",
      yref = "paper"),
    margin = list(
      l = 0,
      r = 0,
      t = 30,
      b = 0
    ),
    font   = t,
    scene3 = list(xaxis = axx,
                  yaxis = axy,
                  zaxis = axz2,
                  camera = camera))

p7c_mean <- plot_ly(x = seq(0, 100, by = 5),
                    y = seq(10, 35, by = 1),
                    z = ~matrix_past_stoch_mean,
                    scene = "scene4",
                    showscale = FALSE) %>%
  add_surface() %>%
  layout(
    title = list(
      text = "<br><b>c)</b>",
      font = list(
        size = 26
      ),
      x    = 0,
      xref = "paper",
      yref = "paper"),
    margin = list(
      l = 0,
      r = 0,
      t = 30,
      b = 0
    ),
    font   = t,
    scene4 = list(xaxis = axx,
                  yaxis = axy,
                  zaxis = axz1,
                  camera = camera))

p7d_mean <- plot_ly(x = seq(0, 100, by = 5),
                    y = seq(10.001, 35, by = 1),
                    z = ~matrix_past_stoch_generations_mean,
                    scene = "scene5",
                    showscale = FALSE) %>%
  add_surface() %>%
  layout(
    title = list(
      text = "<br><b>d)</b>",
      font = list(
        size = 26
      ),
      x    = 0,
      xref = "paper",
      yref = "paper"),
    margin = list(
      l = 0,
      r = 0,
      t = 30,
      b = 0
    ),
    font   = t,
    scene5 = list(xaxis = axx,
                  yaxis = axy,
                  zaxis = axz2,
                  camera = camera))

# plotly::orca(p7a_mean, file = "/plots/2021_05_05_branch_new_mortality_relative/fig7a_size16.png",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p7b_mean, file = "/plots/2021_05_05_branch_new_mortality_relative/fig7b_size16.png",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p7c_mean, file = "/plots/2021_05_05_branch_new_mortality_relative/fig7c_size16.png",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p7d_mean, file = "/plots/2021_05_05_branch_new_mortality_relative/fig7d_size16.png",
#              scale = 20, width = 600, height = 580)


#------------------------------------------------------------------------------

p7a_median <- plot_ly(x = seq(0, 100, by = 5),
                      y = seq(10, 35, by = 1),
                      z = ~matrix_past_median,
                      scene = "scene2",
                      showscale = FALSE) %>%
  add_surface() %>%
    layout(
      title = list(
        text = "<br><b>a)</b>",
        font = list(
          size = 26
        ),
        x    = 0,
        xref = "paper",
        yref = "paper"),
      margin = list(
        l = 0,
        r = 0,
        t = 30,
        b = 0
      ),
      font   = t,
      scene2 = list(xaxis = axx,
                  yaxis = axy,
                  zaxis = axz1,
                  camera = camera))



p7b_median <- plot_ly(x = seq(0, 100, by = 5),
                      y = seq(10.0001, 35, by = 1),
                      z = ~matrix_past_generations_median,
                      scene = "scene3",
                      showscale = FALSE) %>%
  add_surface() %>%
  layout(
    title = list(
      text = "<br><b>b)</b>",
      font = list(
        size = 26
      ),
      x    = 0,
      xref = "paper",
      yref = "paper"),
    margin = list(
      l = 0,
      r = 0,
      t = 30,
      b = 0
    ),
    font   = t,
    scene3 = list(xaxis = axx,
                  yaxis = axy,
                  zaxis = axz2,
                  camera = camera))

p7c_median <- plot_ly(x = seq(0, 100, by = 5),
                      y = seq(10, 35, by = 1),
                      z = ~matrix_past_stoch_median,
                      scene = "scene4",
                      showscale = FALSE) %>%
  add_surface() %>%
  layout(
    title = list(
      text = "<br><b>c)</b>",
      font = list(
        size = 26
      ),
      x    = 0,
      xref = "paper",
      yref = "paper"),
    margin = list(
      l = 0,
      r = 0,
      t = 30,
      b = 0
    ),
    font   = t,
    scene4 = list(xaxis = axx,
                  yaxis = axy,
                  zaxis = axz1,
                  camera = camera))

p7d_median <- plot_ly(x = seq(0, 100, by = 5),
                      y = seq(10.0001, 35, by = 1),
                      z = ~matrix_past_stoch_generations_median,
                      scene = "scene5",
                      showscale = FALSE) %>%
  add_surface() %>%
  layout(
    title = list(
      text = "<br><b>d)</b>",
      font = list(
        size = 26
      ),
      x    = 0,
      xref = "paper",
      yref = "paper"),
    margin = list(
      l = 0,
      r = 0,
      t = 30,
      b = 0
    ),
    font   = t,
    scene5 = list(xaxis = axx,
                  yaxis = axy,
                  zaxis = axz2,
                  camera = camera))

# plotly::orca(p7a_median, file = "/plots/figs12_a_median.png",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p7b_median, file = "/plots/figs12_b_median.png",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p7c_median, file = "/plots/figs12_c_median.png",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p7d_median, file = "/plots/figs12_d_median.png",
#              scale = 20, width = 600, height = 580)

#-----------------------------------------------------------------------------