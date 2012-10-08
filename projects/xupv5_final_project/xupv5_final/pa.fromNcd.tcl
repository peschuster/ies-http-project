
# PlanAhead Launch Script for Post PAR Floorplanning, created by Project Navigator

create_project -name xupv5_final -dir "H:/studium/ies-project/projects/xupv5_final_project/xupv5_final/planAhead_run_1" -part xc5vlx110tff1136-1
set srcset [get_property srcset [current_run -impl]]
set_property design_mode GateLvl $srcset
set_property edif_top_file "H:/studium/ies-project/projects/xupv5_final_project/xupv5_final/system_top.ngc" [ get_property srcset [ current_run ] ]
add_files -norecurse { {H:/studium/ies-project/projects/xupv5_final_project/xupv5_final} {../../xupv5_final/implementation} }
add_files [list {H:/studium/ies-project/projects/xupv5_final_project/xupv5_final/system.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {H:/studium/ies-project/projects/xupv5_final_project/xupv5_final/system_clock_generator_0_wrapper.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {H:/studium/ies-project/projects/xupv5_final_project/xupv5_final/system_ddr2_sdram_wrapper.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {H:/studium/ies-project/projects/xupv5_final_project/xupv5_final/system_dlmb_wrapper.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {H:/studium/ies-project/projects/xupv5_final_project/xupv5_final/system_hard_ethernet_mac_wrapper.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {H:/studium/ies-project/projects/xupv5_final_project/xupv5_final/system_ilmb_wrapper.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {H:/studium/ies-project/projects/xupv5_final_project/xupv5_final/system_microblaze_0_wrapper.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {../../xupv5_final/implementation/system_clock_generator_0_wrapper.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {../../xupv5_final/implementation/system_ddr2_sdram_wrapper.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {../../xupv5_final/implementation/system_dlmb_wrapper.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {../../xupv5_final/implementation/system_hard_ethernet_mac_wrapper.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {../../xupv5_final/implementation/system_ilmb_wrapper.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {../../xupv5_final/implementation/system_microblaze_0_wrapper.ncf}] -fileset [get_property constrset [current_run]]
set_property target_constrs_file "system_top.ucf" [current_fileset -constrset]
add_files [list {system_top.ucf}] -fileset [get_property constrset [current_run]]
link_design
read_xdl -file "H:/studium/ies-project/projects/xupv5_final_project/xupv5_final/system_top.ncd"
if {[catch {read_twx -name results_1 -file "H:/studium/ies-project/projects/xupv5_final_project/xupv5_final/system_top.twx"} eInfo]} {
   puts "WARNING: there was a problem importing \"H:/studium/ies-project/projects/xupv5_final_project/xupv5_final/system_top.twx\": $eInfo"
}
