# load necessary packages
if (!require("pacman")) install.packages("pacman")
pacman::p_load(dplyr, ggplot2, cowplot)

# relative path
data_relative_path <- "../data/out/out_fig4/"

# scenario simulation to be included in the Figure
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



# No fires (figure 4a)
df_no_fire <- data_list[[1]]

colnames(df_no_fire)[2] <- "run_num"
colnames(df_no_fire)[3] <- "year"
colnames(df_no_fire)[18] <- "total_viable_seeds"

no_fire <- df_no_fire %>% 
  group_by(run_num, year) %>% 
  summarise(total_seeds = sum(total_viable_seeds))

no_fire <- no_fire %>% 
  group_by(year) %>% 
  summarise(mean_seeds = mean(total_seeds),
            median_seeds = median(total_seeds),
            sd_seeds = sd(total_seeds),
            plus_sd = mean_seeds + sd_seeds,
            ymin_nolimit = mean_seeds - sd_seeds,
            minus_sd = ifelse(ymin_nolimit < 0, 0, ymin_nolimit))

# Deterministic fires (figure 4b)
fire_deter <- data_list[[2]]

colnames(fire_deter)[2] <- "run_num"
colnames(fire_deter)[3] <- "year"
colnames(fire_deter)[18] <- "total_viable_seeds"

fire_deter <- fire_deter %>% 
  group_by(run_num, year) %>% 
  summarise(total_seeds = sum(total_viable_seeds))

fire_deter <- fire_deter %>% 
  group_by(year) %>% 
  summarise(mean_seeds = mean(total_seeds),
            median_seeds = median(total_seeds),
            sd_seeds = sd(total_seeds),
            plus_sd = mean_seeds + sd_seeds,
            ymin_nolimit = mean_seeds - sd_seeds,
            minus_sd = ifelse(ymin_nolimit < 0, 0, ymin_nolimit))


# Stochastic fires (figure 4c)
fire_stoch <- data_list[[3]]

colnames(fire_stoch)[2] <- "run_num"
colnames(fire_stoch)[3] <- "year"
colnames(fire_stoch)[18] <- "total_viable_seeds"

fire_stoch <- fire_stoch %>% 
  group_by(run_num, year) %>% 
  summarise(total_seeds = sum(total_viable_seeds))

fire_stoch <- fire_stoch %>% 
  group_by(year) %>% 
  summarise(mean_seeds = mean(total_seeds),
            median_seeds = median(total_seeds),
            sd_seeds = sd(total_seeds),
            plus_sd = mean_seeds + sd_seeds,
            ymin_nolimit = mean_seeds - sd_seeds,
            minus_sd = ifelse(ymin_nolimit < 0, 0, ymin_nolimit))


#------------------------------------------------------------------------
# Plotting Figure 4

p_xlab <- xlab("Time (years)")
p_ylab <- ylab("# of viable seeds in the population") 

p_theme <-theme(text            = element_text(size = 9),
                axis.text       = element_text(size = 9),
                axis.title      = element_text(size = 9, face = "bold"),
                legend.text     = element_text(size = 7),
                legend.title    = element_text(size = 7, face = "bold"))

p_yscale <- scale_y_continuous(breaks = seq(0, 500000, by = 100000))
p_cartesian <- coord_cartesian(ylim = c(0, 500000),
                               xlim = c(0, 100))



p1 <- ggplot(data = no_fire) +
  geom_line(aes(x = year, 
                y = mean_seeds),
            size = 0.5,
            colour = "#999999") + 
  geom_ribbon(aes(x = year,
                  ymin = minus_sd,
                  ymax = plus_sd),
              fill = "#999999",
              alpha = 0.3) +
  ggtitle("a)") +
  theme_classic() + p_xlab + p_ylab + p_yscale + p_cartesian + p_theme


p2 <- ggplot(data = fire_deter) +
  geom_line(aes(x = year, 
                y = mean_seeds),
            size = 0.5,
            colour = "#999999") + 
  geom_ribbon(aes(x = year,
                  ymin = minus_sd,
                  ymax = plus_sd),
              fill = "#999999",
              alpha = 0.3) +
  ggtitle("b)") +
  theme_classic() + p_xlab + p_ylab + p_yscale + p_cartesian + p_theme


p3 <- ggplot(data = fire_stoch) +
  geom_line(aes(x = year, 
                y = mean_seeds),
            size = 0.5,
            colour = "#999999") + 
  geom_ribbon(aes(x = year,
                  ymin = minus_sd,
                  ymax = plus_sd),
              fill = "#999999",
              alpha = 0.3) +
  ggtitle("c)") +
  theme_classic() + p_xlab + p_ylab + p_yscale + p_cartesian + p_theme


cowplot::plot_grid(p1, p2, p3, ncol = 1)

#Save the plot. Uncomment lines to save it.
# cowplot::save_plot(
#   filename = "fig4_100.jpg",
#   plot     = ggplot2::last_plot(),
#   path     = "./plots/",
#   base_width    = 6.31,
#   base_height   = (2.8 * 3)
# )

#Save the plot. Uncomment lines to save it.
# cowplot::save_plot(
#   filename = "fig4_small_100.jpg",
#   plot     = ggplot2::last_plot(),
#   path     = "./plots/",
#   base_width    = 3.31,
#   base_height   = (2.8 * 3)
# )

################################################################################

#### inter-fire cohorts #######


# No fires (figure 4a)
df_nofire_intercoh <- df_no_fire %>% 
  group_by(is_postfire_coh, run_num, year) %>% 
  summarise(total_seeds = sum(total_viable_seeds))


df_nofire_intercoh$is_postfire_coh <- as.factor(df_nofire_intercoh$is_postfire_coh)

df_nofire_intercoh$is_postfire_coh <- ordered(df_nofire_intercoh$is_postfire_coh,
                                                  levels = c("1", "0"))

df_nofire_intercoh_summ <- df_nofire_intercoh %>% 
  group_by(is_postfire_coh, year) %>% 
  summarise(mean_seeds   = mean(total_seeds),
            median_seeds = median(total_seeds),
            sd_seeds     = sd(total_seeds),
            plus_sd      = mean_seeds + sd_seeds,
            minus_sd     = ifelse((mean_seeds - sd_seeds) < 0, 
                                  0, 
                                  mean_seeds - sd_seeds),
            q1 = quantile(total_seeds, 0.25),
            q3 = quantile(total_seeds, 0.75))


###################################################################
df_plant_age <- df_no_fire %>% 
  group_by(is_postfire_coh, run_num, year, age) %>% 
  summarise(viable_seeds_per_plant = viable_seeds)


df_plant_age$is_postfire_coh <- as.factor(df_plant_age$is_postfire_coh)

df_plant_age$is_postfire_coh <- ordered(df_plant_age$is_postfire_coh,
                                              levels = c("1", "0"))

df_plant_age_summ <- df_plant_age %>% 
  group_by(is_postfire_coh, year, age) %>% 
  summarise(mean_viable_seeds = mean(viable_seeds_per_plant),
            median_viable_seeds = median(viable_seeds_per_plant),
            sd_seeds     = sd(viable_seeds_per_plant),
            plus_sd      = mean_viable_seeds + sd_seeds,
            minus_sd     = ifelse((mean_viable_seeds - sd_seeds) < 0, 
                                  0, 
                                  mean_viable_seeds - sd_seeds),
            q1 = quantile(viable_seeds_per_plant, 0.25),
            q3 = quantile(viable_seeds_per_plant, 0.75))



##########################################################################

df_plant_age_stoch <- data_list[[3]]

colnames(df_plant_age_stoch)[2] <- "run_num"
colnames(df_plant_age_stoch)[3] <- "year"
colnames(df_plant_age_stoch)[18] <- "total_viable_seeds"

df_plant_age_stoch <- df_plant_age_stoch %>% 
  group_by(is_postfire_coh, run_num, year, age) %>% 
  summarise(viable_seeds_per_plant = viable_seeds)


df_plant_age_stoch$is_postfire_coh <- as.factor(df_plant_age_stoch$is_postfire_coh)

df_plant_age_stoch$is_postfire_coh <- ordered(df_plant_age_stoch$is_postfire_coh,
                                        levels = c("1", "0"))

df_plant_age_stoch_summ <- df_plant_age_stoch %>% 
  group_by(is_postfire_coh, year, age) %>% 
  summarise(mean_viable_seeds = mean(viable_seeds_per_plant),
            median_viable_seeds = median(viable_seeds_per_plant),
            sd_seeds     = sd(viable_seeds_per_plant),
            plus_sd      = mean_viable_seeds + sd_seeds,
            minus_sd     = ifelse((mean_viable_seeds - sd_seeds) < 0, 
                                  0, 
                                  mean_viable_seeds - sd_seeds),
            q1 = quantile(viable_seeds_per_plant, 0.25),
            q3 = quantile(viable_seeds_per_plant, 0.75))


#########################################################################


### 
# plot inter-fire cohort 


p_fire <- geom_vline(mapping = aes(xintercept = 17,
                                   linetype = "mean"))

p_fire_scale <- scale_linetype_manual(name = "Fire interval",
                                      values = c(1))

p_fill_inter <- scale_fill_manual(name = "",
                                  values = c("#56B4E9", 
                                             "#E69F00"),
                                  labels = c("post-fire cohort",
                                             "inter-fire cohorts"))

p_color_inter <- scale_colour_manual(name   = "",
                                     values = c("#56B4E9", 
                                                "#E69F00"),
                                     labels = c("post-fire cohort",
                                                "inter-fire cohorts"))


p_xlab_inter <- xlab("Time since last fire (years)")
p_ylab_inter <- ylab("# of viable seeds")
p_xscale_inter <- scale_x_continuous(breaks = seq(0, 100, 25))
p_yscale_inter <- scale_y_continuous(breaks = seq(0, 500000, by = 100000))
p_cartesian_inter <- coord_cartesian(ylim = c(0, 500000), xlim = c(0, 100))

p_theme2 <- theme(text = element_text(size = 12, face = "bold"),
                  axis.text    = element_text(size = 12),
                  axis.title   = element_text(size = 12, face = "bold"),
                  legend.text  = element_text(size = 12, face = "plain"),
                  legend.title = element_text(size = 12, face = "bold"),
                  legend.position = "bottom",
                  legend.direction = "vertical")

p_line_median_inter <- geom_line(mapping = aes(x = year,
                                               y = median_seeds, 
                                               group = is_postfire_coh, 
                                               colour = is_postfire_coh), 
                                 size = 0.8)

p_ribbon_median_inter <- geom_ribbon(mapping = aes(x = year, 
                                                   ymin = q1, 
                                                   ymax = q3, 
                                                   group = is_postfire_coh, 
                                                   fill = is_postfire_coh), 
                                     alpha = 0.3)

p_line_mean_inter <- geom_line(mapping = aes(x = year, 
                                             y = mean_seeds, 
                                             group = is_postfire_coh, 
                                             colour = is_postfire_coh), 
                               size = 0.8)

p_ribbon_mean_inter <- geom_ribbon(mapping = aes(x = year, 
                                                 ymin = minus_sd, 
                                                 ymax = plus_sd, 
                                                 group = is_postfire_coh, 
                                                 fill = is_postfire_coh), 
                                   alpha = 0.3)



##############################


pS14 <- ggplot(data = df_nofire_intercoh_summ) + 
  p_line_mean_inter + p_ribbon_mean_inter + p_fill_inter + p_color_inter +
  p_xlab_inter + p_ylab_inter + p_xscale_inter + p_yscale_inter +
  p_cartesian_inter + theme_classic() + 
  theme(text = element_text(size = 14, face = "bold"),
        axis.text    = element_text(size = 14),
        axis.title   = element_text(size = 14, face = "bold"),
        legend.text  = element_text(size = 14, face = "plain"),
        legend.title = element_text(size = 14, face = "bold"),
        legend.position = c(0.80,0.65),
        legend.direction = "vertical")


#Save the plot. Uncomment lines to save it.
# cowplot::save_plot(
#   filename = "figS14_intercoh.jpg",
#   plot     = ggplot2::last_plot(),
#   path     = "./plots/",
#   base_width    = 8,
#   base_height   = 4
# )
