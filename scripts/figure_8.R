# load necessary packages
if (!require("pacman")) install.packages("pacman")
pacman::p_load(dplyr, ggplot2, here, cowplot, plotly)

# relative path
data_relative_path <- "../data/out/out_fig8/"

# scenario simulation to be included in the Figure
simulations <- "fig8"

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
df_curr <- data_list[[1]] %>% 
  dplyr::group_by(pollination_mean, fire_mean) %>%
  dplyr::summarize(mean_persistence = mean(persistence_time, na.rm = TRUE),
                   median_persistence = median(persistence_time, na.rm = TRUE),
                   mean_generations = mean(num_generations, na.rm = TRUE),
                   median_generations = median(num_generations, na.rm = TRUE),
                   max_persistence = max(persistence_time, na.rm = TRUE))


# nrow refers to fire_mean from 10 years to 35 years in 1 year steps
# ncol refers to pollination_mean from 0% to 100% in 5% steps
matrix_curr_mean <- matrix(df_curr$mean_persistence,
                           nrow = 26, ncol = 21)

matrix_curr_median <- matrix(df_curr$median_persistence, 
                             nrow = 26, ncol = 21)

matrix_curr_generations_mean <- matrix(df_curr$mean_generations, 
                                       nrow = 26, ncol = 21)

matrix_curr_generations_median <- matrix(df_curr$median_generations, 
                                         nrow = 26, ncol = 21)



# sum total of viable seeds per year per cohort type (i.e., inter-fire and post-fire)
df_curr_stoch <- data_list[[2]] %>% 
  dplyr::group_by(pollination_mean, fire_mean) %>%
  dplyr::summarize(mean_persistence = mean(persistence_time, na.rm = TRUE),
                   median_persistence = median(persistence_time, na.rm = TRUE),
                   mean_generations = mean(num_generations, na.rm = TRUE),
                   median_generations = median(num_generations, na.rm = TRUE),
                   max_persistence = max(persistence_time, na.rm = TRUE))


matrix_curr_stoch_mean <- matrix(
  df_curr_stoch$mean_persistence, nrow = 26, ncol = 21)

matrix_curr_stoch_generations_mean <- matrix(
  df_curr_stoch$mean_generations, nrow = 26, ncol = 21)

matrix_curr_stoch_median <- matrix(
  df_curr_stoch$median_persistence, nrow = 26, ncol = 21)

matrix_curr_stoch_generations_median <- matrix(
  df_curr_stoch$median_generations, nrow = 26, ncol = 21)


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
  start = 12,
  domain = c(10, 39),
  range = c(10, 39)
)

axz1 <- list(
  title = "<b>Persistence (yr)</b>",
  nticks = 6,
  domain = c(0, 150),
  range = c(0, 150)
)

axz2 <- list(
  title = "<b># of generations</b>",
  nticks = 7,
  domain = c(0, 8),
  range = c(0, 8)
)

camera <- list(eye = list(x = -1.25, y = -0.75, z = 1.85))


p8a_mean <- plot_ly(x = seq(0, 100, by = 5),
                    y = seq(10, 35, by = 1),
                    z = ~matrix_curr_mean,
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


p8b_mean <- plot_ly(x = seq(0, 100, by = 5),
                    y = seq(10, 35, by = 1),
                    z = ~matrix_curr_generations_mean,
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

p8c_mean <- plot_ly(x = seq(0, 100, by = 5),
                    y = seq(10, 35, by = 1),
                    z = ~matrix_curr_stoch_mean,
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

p8d_mean <- plot_ly(x = seq(0, 100, by = 5),
                    y = seq(10, 35, by = 1),
                    z = ~matrix_curr_stoch_generations_mean,
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

# plotly::orca(p8a_mean, file = "/plots/fig8a_size16.png",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p8b_mean, file = "/plots/fig8b_size16.png",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p8c_mean, file = "/plots/fig8c_size16.png",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p8d_mean, file = "/plots/fig8d_size16.png",
#              scale = 20, width = 600, height = 580)

# plotly::orca(p8a_mean, file = "/plots/fig8a.svg",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p8b_mean, file = "/plots/fig8b.svg",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p8c_mean, file = "/plots/fig8c.svg",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p8d_mean, file = "/plots/fig8d.svg",
#              scale = 20, width = 600, height = 580)


#------------------------------------------------------------------------------

p8a_median <- plot_ly(x = seq(0, 100, by = 5),
                      y = seq(10, 35, by = 1),
                      z = ~matrix_curr_median,
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



p8b_median <- plot_ly(x = seq(0, 100, by = 5),
                      y = seq(10, 35, by = 1),
                      z = ~matrix_curr_generations_median,
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

p8c_median <- plot_ly(x = seq(0, 100, by = 5),
                      y = seq(10, 35, by = 1),
                      z = ~matrix_curr_stoch_median,
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

p8d_median <- plot_ly(x = seq(0, 100, by = 5),
                      y = seq(10, 35, by = 1),
                      z = ~matrix_curr_stoch_generations_median,
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

# plotly::orca(p8a_median, file = "/plots/figs13_a_median.png",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p8b_median, file = "/plots/figs13_b_median.png",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p8c_median, file = "/plots/figs13_c_median.png",
#              scale = 20, width = 600, height = 580)
# 
# plotly::orca(p8d_median, file = "/plots/figs13_d_median.png",
#              scale = 20, width = 600, height = 580)

#-----------------------------------------------------------------------------