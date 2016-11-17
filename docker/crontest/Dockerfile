FROM ubuntu:16.04
MAINTAINER poul@poulsander.com

#Based on https://www.ekito.fr/people/run-a-cron-job-with-docker/

#Install Cron
RUN apt-get update && apt-get -y install cron

# Add crontab file in the cron directory
COPY crontab /etc/cron.d/hello-cron
 
# Give execution rights on the cron job
RUN chmod 0644 /etc/cron.d/hello-cron
 
# Create the log file to be able to run tail
RUN touch /var/log/cron.log && chmod o+w /var/log/cron.log

RUN chmod o+w /var/run
 
# Run the command on container startup
CMD cron && tail -f /var/log/cron.log
