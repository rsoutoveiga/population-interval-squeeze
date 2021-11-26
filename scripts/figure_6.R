# load necessary packages
if (!require("pacman")) install.packages("pacman")
pacman::p_load(dplyr, ggplot2, here, cowplot)

# relative path
data_relative_path <- "../data/out/out_fig6/baseline/"

# sim_id simulation to be included in the Figure
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

# merge all the df
all_data_bind <- bind_rows(data_list, .id = "sim_id")


figure_6_base <- all_data_bind %>%
  group_by(sim_id, pollination_mean) %>% 
  summarise(mean_time = mean(persistence_time, na.rm = TRUE),
            median_time = median(persistence_time, na.rm = TRUE),
            sd_time = sd(persistence_time, na.rm = TRUE),
            plus_sd = ifelse((mean_time + sd_time) > 500, 500, mean_time + sd_time),
            minus_sd = ifelse((mean_time - sd_time) < 0, 0, mean_time - sd_time),
            q1 = quantile(persistence_time, 0.25),
            q3 = quantile(persistence_time, 0.75))

##############################################################################
# current climate sim_id
# relative path
data_relative_path <- "../data/out/out_fig6/current/"

# sim_id simulation to be included in the Figure
simulations <- "sim"

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

# merge all the df
all_data_bind <- bind_rows(data_list, .id = "sim_id")


figure_6_curr <- all_data_bind %>%
  group_by(sim_id, pollination_mean) %>% 
  summarise(mean_time = mean(persistence_time, na.rm = TRUE),
            median_time = median(persistence_time, na.rm = TRUE),
            sd_time = sd(persistence_time, na.rm = TRUE),
            plus_sd = ifelse((mean_time + sd_time) > 500, 500, mean_time + sd_time),
            minus_sd = ifelse((mean_time - sd_time) < 0, 0, mean_time - sd_time),
            q1 = quantile(persistence_time, 0.25),
            q3 = quantile(persistence_time, 0.75))

##############################################################################
# percentage changes from past and present pollination success:

# 1. baseline climate, deterministic fires = 0%
(figure_6_base$mean_time[figure_6_base$sim_id == "sim_persistence_0" &
                          figure_6_base$pollination_mean == 65] -
figure_6_base$mean_time[figure_6_base$sim_id == "sim_persistence_0" &
                          figure_6_base$pollination_mean == 90]) /
  figure_6_base$mean_time[figure_6_base$sim_id == "sim_persistence_0" &
                            figure_6_base$pollination_mean == 90] * 100
  
# 2. baseline climate, stochastic fires = -4.11% 
(figure_6_base$mean_time[figure_6_base$sim_id == "sim_persistence_1" &
                           figure_6_base$pollination_mean == 65] -
    figure_6_base$mean_time[figure_6_base$sim_id == "sim_persistence_1" &
                              figure_6_base$pollination_mean == 90]) /
  figure_6_base$mean_time[figure_6_base$sim_id == "sim_persistence_1" &
                            figure_6_base$pollination_mean == 90] * 100

# 3. current climate, deterministic fires = -16.09%
(figure_6_curr$mean_time[figure_6_curr$sim_id == "sim_persistence_2" &
                           figure_6_curr$pollination_mean == 65] -
    figure_6_curr$mean_time[figure_6_curr$sim_id == "sim_persistence_2" &
                              figure_6_curr$pollination_mean == 90]) /
  figure_6_curr$mean_time[figure_6_curr$sim_id == "sim_persistence_2" &
                            figure_6_curr$pollination_mean == 90] * 100

# 4. current climate, stochastic fires = -17.93%
(figure_6_curr$mean_time[figure_6_curr$sim_id == "sim_persistence_3" &
                           figure_6_curr$pollination_mean == 65] -
    figure_6_curr$mean_time[figure_6_curr$sim_id == "sim_persistence_3" &
                              figure_6_curr$pollination_mean == 90]) /
  figure_6_curr$mean_time[figure_6_curr$sim_id == "sim_persistence_3" &
                            figure_6_curr$pollination_mean == 90] * 100


##############################################################################
## Plotting

p_fill <- scale_fill_manual(name = "Fire events",
                            values = c("#56B4E9", "#E69F00"),
                            labels = c("deterministic", "stochastic"))

p_color <- scale_color_manual(name = "Fire events",
                              values = c("#56B4E9", "#E69F00"),
                              labels = c("deterministic", "stochastic"))

p_xlab <- xlab("Pollination success (%)")
p_ylab <- ylab("Persistence time (years)")
# p_xscale <- scale_x_continuous(breaks = seq(0, 100, 20))
p_yscale <- scale_y_continuous(breaks = seq(0, 500, by = 100))
p_cartesian <- coord_cartesian(ylim = c(0, 500))

p_theme <-theme(text            = element_text(size = 9),
                axis.text       = element_text(size = 9),
                axis.title      = element_text(size = 9, face = "bold"),
                legend.text     = element_text(size = 9),
                legend.title    = element_text(size = 9, face = "bold"),
                legend.position = "none")

p_vline1 <- geom_vline(aes(xintercept = 90, 
                           linetype   = "past"), 
                           colour = "black") 

p_vline2 <- geom_vline(aes(xintercept = 65, 
                           linetype   = "current"), 
                       colour = "black")

p_linescale <- scale_linetype_manual(name   = "Pollination success", 
                                     values = c(1, 2)) 



p2 <- ggplot(data = figure_6_base) +
  geom_line(mapping = aes(x = pollination_mean,
                          y = median_time,
                          group = sim_id,
                          colour = sim_id),
            size = 0.5) + 
  geom_ribbon(mapping = aes(x = pollination_mean,
                            ymin = q1,
                            ymax = q3,
                            group = sim_id,
                            fill = sim_id),
              alpha = 0.3) +
  theme_classic() + p_xlab + p_ylab + p_yscale +
  p_vline1 + p_vline2 + p_linescale + p_fill + p_color +
  p_cartesian  +
  ggtitle("a)") +
  theme(text            = element_text(size = 12),
        axis.text       = element_text(size = 12),
        axis.title      = element_text(size = 12, face = "bold"),
        legend.text     = element_text(size = 9),
        legend.title    = element_text(size = 9, face = "bold"),
        legend.position = "bottom")
p2


# ggsave("fig3_base_median.jpg", 
#        path = "./figures/05_fig3/baseline/",
#        width = 8, 
#        height = 4)


p1 <- ggplot(data = figure_6_base) +
  geom_line(mapping = aes(x = pollination_mean,
                          y = mean_time,
                          group = sim_id,
                          colour = sim_id),
            size = 0.5) + 
  geom_ribbon(mapping = aes(x = pollination_mean,
                            ymin = plus_sd,
                            ymax = minus_sd,
                            group = sim_id,
                            fill = sim_id),
              alpha = 0.3) +
  theme_classic() + p_xlab + p_ylab + p_yscale +
  p_vline1 + p_vline2 + p_linescale + p_fill + p_color +
  p_cartesian + p_theme + ggtitle("a)")


# ggsave("fig3_base_mean.jpg", 
#        path = "./figures/05_fig3/baseline/",
#        width = 8, 
#        height = 4)

p3 <- ggplot(data = figure_6_curr) +
  geom_line(mapping = aes(x = pollination_mean,
                          y = median_time,
                          group = sim_id,
                          colour = sim_id),
            size = 0.5) + 
  geom_ribbon(mapping = aes(x = pollination_mean,
                            ymin = q1,
                            ymax = q3,
                            group = sim_id,
                            fill = sim_id),
              alpha = 0.3) +
  theme_classic() + p_xlab + p_ylab + 
  scale_y_continuous(breaks = seq(0, 500, by = 25)) +
  p_vline1 + p_vline2 + p_linescale + p_fill + p_color +
  coord_cartesian(ylim = c(0, 100)) + p_theme +
  ggtitle("b)")


# ggsave("fig3_base_median.jpg", 
#        path = "./figures/05_fig3/baseline/",
#        width = 8, 
#        height = 4)


p4 <- ggplot(data = figure_6_curr) +
  geom_line(mapping = aes(x = pollination_mean,
                          y = mean_time,
                          group = sim_id,
                          colour = sim_id),
            size = 0.5) + 
  geom_ribbon(mapping = aes(x = pollination_mean,
                            ymin = plus_sd,
                            ymax = minus_sd,
                            group = sim_id,
                            fill = sim_id),
              alpha = 0.3) +
  theme_classic() + p_xlab + p_ylab + 
  scale_y_continuous(breaks = seq(0, 500, by = 50)) +
  p_vline1 + p_vline2 + p_linescale + p_fill + p_color +
  coord_cartesian(ylim = c(0, 175)) + p_theme + ggtitle("b)")


# ggsave("fig3_base_mean.jpg", 
#        path = "./figures/05_fig3/baseline/",
#        width = 8, 
#        height = 4)

plegend <- cowplot::get_legend(p2)

pgrid <- cowplot::plot_grid(p1, p4, ncol = 2)

p <- plot_grid(pgrid, plegend, ncol = 1, rel_widths = c(1, .1), rel_heights = c(1, .1))



#Save the plot. Uncomment lines to save it.
# cowplot::save_plot(
#   filename = "fig6.jpg",
#   plot     = ggplot2::last_plot(),
#   path     = "./plots",
#   base_width    = 3.31 * 2,
#   base_height   = 3
# )


##############################################################################

## Median
# Fig S11

p1 <- ggplot(data = figure_6_base) +
  geom_line(mapping = aes(x = pollination_mean,
                          y = median_time,
                          group = sim_id,
                          colour = sim_id),
            size = 0.5) + 
  geom_ribbon(mapping = aes(x = pollination_mean,
                            ymin = q1,
                            ymax = q3,
                            group = sim_id,
                            fill = sim_id),
              alpha = 0.3) +
  theme_classic() + p_xlab + p_ylab + p_yscale +
  p_vline1 + p_vline2 + p_linescale + p_fill + p_color +
  p_cartesian + p_theme + ggtitle("a)")


# ggsave("fig3_base_mean.jpg", 
#        path = "./figures/05_fig3/baseline/",
#        width = 8, 
#        height = 4)

p3 <- ggplot(data = figure_6_curr) +
  geom_line(mapping = aes(x = pollination_mean,
                          y = median_time,
                          group = sim_id,
                          colour = sim_id),
            size = 0.5) + 
  geom_ribbon(mapping = aes(x = pollination_mean,
                            ymin = q1,
                            ymax = q3,
                            group = sim_id,
                            fill = sim_id),
              alpha = 0.3) +
  theme_classic() + p_xlab + p_ylab + 
  scale_y_continuous(breaks = seq(0, 500, by = 25)) +
  p_vline1 + p_vline2 + p_linescale + p_fill + p_color +
  coord_cartesian(ylim = c(0, 100)) + p_theme +
  ggtitle("b)")



p4 <- ggplot(data = figure_6_curr) +
  geom_line(mapping = aes(x = pollination_mean,
                          y = median_time,
                          group = sim_id,
                          colour = sim_id),
            size = 0.5) + 
  geom_ribbon(mapping = aes(x = pollination_mean,
                            ymin = q1,
                            ymax = q3,
                            group = sim_id,
                            fill = sim_id),
              alpha = 0.3) +
  theme_classic() + p_xlab + p_ylab + 
  scale_y_continuous(breaks = seq(0, 500, by = 50)) +
  p_vline1 + p_vline2 + p_linescale + p_fill + p_color +
  coord_cartesian(ylim = c(0, 175)) + p_theme + ggtitle("b)")


# ggsave("fig3_base_mean.jpg", 
#        path = "./figures/05_fig3/baseline/",
#        width = 8, 
#        height = 4)

plegend <- cowplot::get_legend(p2)

pgrid <- cowplot::plot_grid(p1, p4, ncol = 2)

p <- plot_grid(pgrid, plegend, ncol = 1, rel_widths = c(1, .1), rel_heights = c(1, .1))

#Save the plot. Uncomment lines to save it.
# cowplot::save_plot(
#   filename = "figS11_median.jpg",
#   plot     = ggplot2::last_plot(),
#   path     = "./plots",
#   base_width    = 3.31 * 2,
#   base_height   = 3
# )