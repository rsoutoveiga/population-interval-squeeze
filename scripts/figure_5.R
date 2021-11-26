# load necessary packages
if (!require("pacman")) install.packages("pacman")
pacman::p_load(dplyr, ggplot2, here, cowplot)

# relative path
data_relative_path <- "../data/out/out_fig5/"

# scenario simulation to be included in the Figure
simulations <- "sim"

# names of the .csv files
names_files <- list.files(path = here::here(data_relative_path),
                          pattern = paste0(simulations,
                                           ".*\\.csv$"))

# read all the .csv files for the analysis
data_list <- lapply(paste0(data_relative_path, names_files), read.csv)

# names of the dataframes, i.e., remove extension file (.csv)
names_df <- gsub(".csv", "", names_files)

# Rename data frames within the list with the name files
names(data_list) <- names_df


#############################################

# merge all the df
all_data_bind <- bind_rows(data_list, .id = "id")

figure_5 <- all_data_bind %>% 
  group_by(sim_id, run_num, year) %>% 
  summarise(total_seeds = sum(total_viable_seeds))

figure_5$sim_id <- as.factor(figure_5$sim_id)

figure_5 <- figure_5 %>% 
  group_by(sim_id, year) %>% 
  summarise(mean_seeds   = mean(total_seeds),
            median_seeds = median(total_seeds),
            sd_seeds     = sd(total_seeds),
            plus_sd      = mean_seeds + sd_seeds,
            minus_sd     = ifelse((mean_seeds - sd_seeds) < 0, 
                                  0, 
                                  mean_seeds - sd_seeds),
            q1 = quantile(total_seeds, 0.25),
            q3 = quantile(total_seeds, 0.75))

#------------------------------------------------------------------------
# Plotting Figure 5

p_fire <- geom_vline(mapping = aes(xintercept = 17,
                                   linetype = "mean"),
                     size = 0.4)

p_fire_scale <- scale_linetype_manual(name = "Fire interval",
                                      values = c(1))

p_fill_scn4 <- scale_fill_manual(name = "Climate, pollination",
                                 values = c("#999999", 
                                            "#009E73",
                                            "#56B4E9", 
                                            "#E69F00"),
                                 labels = c("1988\u20132002, 90%",
                                            "1988\u20132002, 65%",
                                            "2003\u20132017, 90%",
                                            "2003\u20132017, 65%"))

p_color_scn4 <- scale_colour_manual(name   = "Climate, pollination",
                                    values = c("#999999", 
                                               "#009E73",
                                               "#56B4E9", 
                                               "#E69F00"),
                                    labels = c("1988\u20132002, 90%",
                                               "1988\u20132002, 65%",
                                               "2003\u20132017, 90%",
                                               "2003\u20132017, 65%"))

p_fill_scn3 <-   scale_fill_manual(name   = "Climate, pollination",
                                   values = c("#999999", "#56B4E9", "#E69F00"),
                                   labels = c("1988\u20132002, 90%",
                                              "2003\u20132017, 90%",
                                              "2003\u20132017, 65%"))

p_color_scn3 <- scale_colour_manual(name   = "Climate, pollination",
                                    values = c("#999999", "#56B4E9", "#E69F00"),
                                    labels = c("1988\u20132002, 90%",
                                               "2003\u20132017, 90%",
                                               "2003\u20132017, 65%"))

p_xlab <- xlab("Time since last fire (years)")
p_ylab <- ylab("# of viable seeds in the population")
p_xscale <- scale_x_continuous(breaks = seq(0, 50, 10))
p_yscale <- scale_y_continuous(breaks = seq(0, 500000, by = 100000))
p_cartesian <- coord_cartesian(ylim = c(0, 510000), xlim = c(0, 50))

p_theme <-theme(legend.position = c(0.83,0.75),
                text            = element_text(size = 9), 
                axis.text       = element_text(size = 9),
                axis.title      = element_text(size = 9, face = "bold"),
                legend.text     = element_text(size = 7),
                legend.title    = element_text(size = 7, face = "bold"),
                legend.key.size = unit(4, "mm"),
                legend.margin   = margin(c(1, 1, 1, 1)))


p_line_median <- geom_line(mapping = aes(x = year, 
                                         y = median_seeds, 
                                         group = sim_id,
                                         colour = sim_id), 
                           size = 0.5)

p_ribbon_median <- geom_ribbon(mapping = aes(x = year, 
                                             ymin = q1, 
                                             ymax = q3, 
                                             group = sim_id, 
                                             fill = sim_id), 
                               alpha = 0.3)

p_line_mean <- geom_line(mapping = aes(x = year, 
                                       y = mean_seeds, 
                                       group = sim_id, 
                                       colour = sim_id), 
                         size = 0.5)

p_ribbon_mean <- geom_ribbon(mapping = aes(x = year, 
                                           ymin = minus_sd, 
                                           ymax = plus_sd, 
                                           group = sim_id, 
                                           fill = sim_id), 
                             alpha = 0.3)


p1 <- ggplot(data = figure_5) +
  p_line_mean + p_ribbon_mean +
  theme_classic() + p_xlab + p_ylab + p_xscale + p_yscale +
  p_fire + p_fire_scale + p_fill_scn4 + p_color_scn4 +
  p_cartesian + p_theme

#Save the plot. Uncomment lines to save it.
# cowplot::save_plot(
#   filename = "fig5.jpg",
#   plot     = ggplot2::last_plot(),
#   path     = "./plots",
#   base_width    = 3.31,
#   base_height   = 2.8
# )

# how to plot a shared legend
# https://stackoverflow.com/questions/37335709/one-shared-legend-for-a-cowplot-grid-in-r
p2 <- ggplot(data = figure_5) + 
  p_line_median + p_ribbon_median +
  theme_classic() + p_xlab + p_ylab + p_xscale + p_yscale +
  p_fire + p_fire_scale + p_fill_scn4 + p_color_scn4 +
  p_cartesian + p_theme

# cowplot::save_plot(
#   filename = "figS10_median.jpg",
#   plot     = ggplot2::last_plot(),
#   path     = "./plots",
#   base_width    = 3.31,
#   base_height   = 2.8
# )

# ggsave("fig2_scn4_cone1_median_one_var.jpg", 
#        path = "./figures/04_fig2/lmer_one_variable/",
#        width = 8, 
#        height = 4)


